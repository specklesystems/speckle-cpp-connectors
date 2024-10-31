using System.IO.Compression;
using Build;
using GlobExpressions;
using static Bullseye.Targets;
using static SimpleExec.Command;

const string CLEAN = "clean";
const string BUILD = "build";
const string ZIP = "zip";
const string RESTORE_TOOLS = "restore-tools";
const string BUILD_SERVER_VERSION = "build-server-version";

//need to pass arguments
/*var arguments = new List<string>();
if (args.Length > 1)
{
  arguments = args.ToList();
  args = new[] { arguments.First() };
  //arguments = arguments.Skip(1).ToList();
}*/

Target(
  CLEAN,
  ForEach("**/output"),
  dir =>
  {
    IEnumerable<string> GetDirectories(string d)
    {
      return Glob.Directories(".", d);
    }

    void RemoveDirectory(string d)
    {
      if (Directory.Exists(d))
      {
        Console.WriteLine(d);
        Directory.Delete(d, true);
      }
    }

    foreach (var d in GetDirectories(dir))
    {
      RemoveDirectory(d);
    }
  }
);

Target(
  RESTORE_TOOLS,
  () =>
  {
    Run("dotnet", "tool restore");
  }
);

Target(
  BUILD_SERVER_VERSION,
  DependsOn(RESTORE_TOOLS),
  () =>
  {
    Run("dotnet", "tool run dotnet-gitversion /output json /output buildserver");
  }
);

Target(
  BUILD,
  Consts.Solutions,
  s =>
  {
    //var version = Environment.GetEnvironmentVariable("GitVersion_FullSemVer") ?? "3.0.0-localBuild";
    //var fileVersion = Environment.GetEnvironmentVariable("GitVersion_AssemblySemFileVer") ?? "3.0.0.0";
    //Console.WriteLine($"Version: {version} & {fileVersion}");
    //Run("dotnet", $"build {s} -c Release --no-restore -p:Version={version} -p:FileVersion={fileVersion} -v:m");
    Run("msbuild", $"{s} /p:Configuration=ReleaseAC27");
  }
);

Target(
  ZIP,
  DependsOn(BUILD),
  Consts.InstallerManifests,
  x =>
  {
    var outputDir = Path.Combine(".", "output");
    var slugDir = Path.Combine(outputDir, x.HostAppSlug);

    Directory.CreateDirectory(outputDir);
    Directory.CreateDirectory(slugDir);

    foreach (var asset in x.Projects)
    {
      var fullPath = Path.Combine(".", asset.ProjectPath, asset.TargetName);
      if (!Directory.Exists(fullPath))
      {
        throw new InvalidOperationException("Could not find: " + fullPath);
      }

      var assetName = Path.GetFileName(asset.ProjectPath);
      var connectorDir = Path.Combine(slugDir, assetName);

      Directory.CreateDirectory(connectorDir);
      foreach (var directory in Directory.EnumerateDirectories(fullPath, "*", SearchOption.AllDirectories))
      {
        Directory.CreateDirectory(directory.Replace(fullPath, connectorDir));
      }

      foreach (var file in Directory.EnumerateFiles(fullPath, "*", SearchOption.AllDirectories))
      {
        Console.WriteLine(file);
        File.Copy(file, file.Replace(fullPath, connectorDir), true);
      }
    }

    var outputPath = Path.Combine(outputDir, $"{x.HostAppSlug}.zip");
    File.Delete(outputPath);
    Console.WriteLine($"Zipping: '{slugDir}' to '{outputPath}'");
    ZipFile.CreateFromDirectory(slugDir, outputPath);
    // Directory.Delete(slugDir, true);
  }
);

Target("default", DependsOn(ZIP), () => Console.WriteLine("Done!"));

await RunTargetsAndExitAsync(args).ConfigureAwait(true);
