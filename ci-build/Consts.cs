namespace Build;

public static class Consts
{
  public static readonly string[] Solutions = ["SpeckleConnector/Connector.sln"];

  public static readonly InstallerProject[] InstallerManifests =
  {
    new("archicad", [new("archicad27", "SpeckleConnector/x64/ReleaseAC27/", "*.apx")])
  };
}

public readonly record struct InstallerProject(string HostAppSlug, IReadOnlyList<InstallerAsset> Projects)
{
  public override string ToString() => $"{HostAppSlug}";
}

public readonly record struct InstallerAsset(string ConnectorVersion, string OutputPath, string GlobPattern = "*");
