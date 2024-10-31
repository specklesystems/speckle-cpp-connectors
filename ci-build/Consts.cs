namespace Build;

public static class Consts
{
  public static readonly string[] Solutions = ["SpeckleConnector/Connector.sln"];

  public static readonly InstallerProject[] InstallerManifests =
  {
    new("archicad", [new("SpeckleConnector", "x64/ReleaseAC27/net6.0-windows")])
  };
}

public readonly record struct InstallerProject(string HostAppSlug, IReadOnlyList<InstallerAsset> Projects)
{
  public override string ToString() => $"{HostAppSlug}";
}

public readonly record struct InstallerAsset(string ProjectPath, string TargetName);
