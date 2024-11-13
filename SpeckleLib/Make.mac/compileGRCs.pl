#! /usr/bin/perl -w

use FindBin ;
use File::Spec::Functions qw(splitpath catfile updir);
use File::Copy;
use Cwd;

my $os;
my $devKitPath;
my $acVersion = $ENV{BIM_VERSION};

if ($^O =~ /MSWin/) {
	$os = "Win";
} else {
	$os = "Mac";
}
$devKitPath = catfile ($FindBin::Bin, updir (), updir (), "ArchiCAD " . $acVersion);


#-----------------------------------------------------------------------
# Strip redundant expressions from the target file

# $targetDirectory: The target directory
# $$targetFile: The target file
#-----------------------------------------------------------------------
sub StripRedundancies ($$)
{
	my ($source, $destination) = @_ ;
	
	open my $fh, "< :raw", $source or die "Cannot open $filename for reading: $!\n";
	my $saved_sep = $/;
	undef $/;
	$content = <$fh>;
	close ($fh) or die "Can't close file: $!";
		#Strip out comments
	$content =~ s { (     [^"'/]*
			+r comment.
			|  "[^\\"]*(?:\\.[^\\"]*)*"
			|  '[^\\']*(?:\\.[^\\']*)*'
			|  / (?![*])
			)
			|
			( /[*] [^*]* (?: [*] [^*/]* )* [*]/ )
		}
		{ $2 ? "" : $1 }gsex;
	open $fh, "+> :raw", $destination or die "Cannot open $filename for writing: $!\n";
	print $fh $content;
	$/ = $saved_sep;
	close ($fh) or die "Can't close file: $!";
} #StripRedundancies


#-----------------------------------------------------------------------
# Convert all image resources to the svg format for AC >= 20

# $intermediaryFile: The target file
#-----------------------------------------------------------------------
sub UpdateSVG($)
{
	my ($target) = @_;
	open my $fh, "< :raw", $target or die "Cannot open $filename for reading: $!\n";
	local $/ = undef;
	$contents = <$fh>;
	close ($fh) or die "Can't close file: $!";
	my @svg_defs = ();
	while ($contents =~ /('GBMP'|'GICN')\s*(\d{5})\s*("[^"]*")\s*\{/sg) 
	{
		push @svg_defs, "'GICN' $2 $3 {\n}\n\n";
	}
	if (@svg_defs) {
		open $fh, "> :raw", $target or die "Cannot open $filename for writing: $!\n";
		print $fh "@ #include \"MDIDs_APICD.h\"\n\n";
		print $fh @svg_defs;
		close ($fh) or die "Can't close file: $!";
	}
} #UpdateSVG


#-----------------------------------------------------------------------
# Executes one GRC conversion command

# tool: Path to the GRC converter tool
# headerDir: Path to headers included in resource files
# sourceDir: Path to the input GRC source directory
# inputFile: Name of the input GRC file
# destinationDir: Path to the output native resource file directory
# localCode: The localisation code, e.g. "EN-GB"
# platformName: Name of the target platform, e.g. "ArchiCAD18"
# resourcePath: Optional path to any supplementary resources, e.g. images
#-----------------------------------------------------------------------
sub DoGRC ($$$$$$$$)
{
	my ($tool, $headerDir, $sourceDir, $inputFile, $destinationDir, $localCode, $platformName, $resourcePath) = @_ ;
		#Get the bare resource name
	my $outputName = $inputFile;
	$outputName =~ s/.grc//;
		#And extract the version number (if found)
	my ($outputNumber) = $outputName =~ /([0-9]+)$/;
		#If it has a version that doesn't match this build, we don't want it
	if ($outputNumber) {
		if ($outputNumber != $ENV{BIM_VERSION}) {
			return;
		}
		$outputName =~ s/$outputNumber\z//;
	}
	my $outputFile = $outputName . ".ro";
	my $imageRes = "";
		#Optional path to a resource directory
	if ($resourcePath ne "") {
		if ($ENV{BIM_VERSION} > 16) {
			$resourcePath = " -p \"" . $resourcePath . "\"";
			if ($ENV{BIM_VERSION} >= 20) {
				$imageRes = " -w 2";
			}
		} else {
			$resourcePath = " -j .roo -p \"" . $resourcePath . "\""
		}
	}
		#Create the destination directory
	if ( $localCode ne "" ) {
		$destinationDir = catfile ($destinationDir, $localCode) ;
	}
	mkdir $destinationDir;
	$destinationDir = catfile ($destinationDir, $platformName);
	mkdir $destinationDir;
		#Run the tool on the file
	print "\t$inputFile\n";
	my $toolOptions = "";
	if ($os eq "Win") {
		$toolOptions = " -m r -q utf8 0 -T W";
	} else {
		$toolOptions = " -m r -q utf8 utf16 -T M -n";
	}
	my $intermediaryFile = catfile ($destinationDir, $outputFile .".i");
	if ($os eq "Win") {
		StripRedundancies(catfile ($sourceDir, $inputFile), $intermediaryFile);
	}
	if ($os eq "Mac") {
		my $frameworkVersion = catfile ($ENV{FRAMEWORK_PATH}, "Speckle" . $acVersion );
		my $incHeaderDir = catfile (Cwd::realpath ($devKitPath), "Support", "Inc");
		system ("xcrun clang -x c++ -E -P -DPREPROCESS_GRC -DGS_64BIT -Dmacintosh -DINT__APP -I \"" . $headerDir . "\" -I \"" . $incHeaderDir . "\" -I \"" . $frameworkVersion . "\" \"" . catfile ($sourceDir, $inputFile) . "\" > \"" . $intermediaryFile . "\"");
	}
	UpdateSVG($intermediaryFile);
	if ($os eq "Win") {
		chdir($devKitPath);
	}
	system ($tool . $toolOptions . " -i \"" . $intermediaryFile . "\"" . $resourcePath . " -o \"" . catfile ($destinationDir, $outputFile) . "\"" . $imageRes);
	unlink($intermediaryFile);
} #DoGRC


#-----------------------------------------------------------------------
# Main - Convert GRC files

my ($localCode) = $ENV{LOCALISATION_SUFFIX};
my $suffix = ".r" . $acVersion;
my $fixed = "RFIX";
my $output = "RO";
my $platform = "ArchiCAD" . $acVersion;
my $baseName = $ENV{BASE_NAME};
my $makeResConvTool;
if ($os eq "Win") {
	$makeResConvTool = catfile ("Support", "Tools", "Win", "ResConv");
} else {
	$makeResConvTool = "\"" . catfile (Cwd::realpath ($devKitPath), "Support", "Tools", "OSX", "ResConv") . "\"";
}
my $projectPath = $ENV{PROJECT_PATH};
my $resTarget = catfile ($ENV{TARGET_BUILD_DIR}, $ENV{UNLOCALIZED_RESOURCES_FOLDER_PATH});
my $roFolder = catfile ($projectPath, $output);
my $rfixFolder = catfile ($projectPath, $fixed);
my $rintFolder = catfile ($projectPath, "RINT", $localCode);
my $srcFolder = catfile ($projectPath, "Src");
my @files;

print "Converting resource files:\n";
mkdir $roFolder;
	#Convert the project fixed resources
print $projectPath . "RFIX\n";
opendir (DIR, catfile ($projectPath, "RFIX"));
@files = readdir (DIR);
closedir (DIR);
foreach $path (@files) {
	($volume,$directories,$file) = splitpath ($path);
	DoGRC ($makeResConvTool, $srcFolder, $rfixFolder, $file, $roFolder, $fixed, $platform, catfile ($projectPath, "RFIX/Images") )  if ($file =~ /.*\.grc$/i);
}
	#Convert the project localisable resources
opendir (DIR, $rintFolder);
@files = readdir (DIR);
closedir (DIR);
foreach $path (@files) {
	($volume,$directories,$file) = splitpath ($path);
	if ((index($file, "Subtype") != -1) || (index($file, "SubType") != -1)) {
		DoGRC ($makeResConvTool, $srcFolder, $rintFolder, $file, $roFolder, $localCode, $platform, catfile ($projectPath, "GDL", $baseName, $localCode, $os, $platform, "Built-in") )  if ($file =~ /.*\.grc$/i);
	} else {
		DoGRC ($makeResConvTool, $srcFolder, $rintFolder, $file, $roFolder, $localCode, $platform, "") if ($file =~ /.*\.grc$/i);
	}
}


#-----------------------------------------------------------------------
# Copy tiff resources

$roFolder = catfile ($projectPath, $output, $fixed, $platform);
opendir (DIR, $roFolder);
@files = readdir (DIR);
closedir (DIR);
mkdir $resTarget;
foreach $file (@files) {
	copy (catfile ($roFolder, $file), $resTarget) if ($file =~ /.*\.tif$/);
}


#-----------------------------------------------------------------------
# Copy localisable resource files and merge string resources to "Localizable.strings"

if (($os eq "Mac") && ($ENV{BIM_VERSION} > 17)) {
	
	$roFolder = catfile ($projectPath, $output, $localCode, $platform);
	
	my $localResources = $ENV{LANGUAGE_NAME} . ".lproj";
	system ("mkdir -p \"" . catfile ($resTarget, $localResources) . "\"") ;
	my $locStrings = catfile ($resTarget, $localResources, "Localizable.strings");
	open (OFILE, ">:encoding(UTF-16LE)", $locStrings) or die "Can't create file: $!";
	my $buf = "\x{feff}";
	print OFILE $buf;
	printf "\t=> Localizable resources\n";
	opendir (DIR, $roFolder);
	@files = readdir (DIR);
	closedir (DIR);
	undef $/; # enable slurp mode
	foreach $file (@files) {
		if ($file =~ /.*\.strings$/) {
			printf "\t\t$file\n";
			my $stringsFile = catfile ($roFolder, $file);
			if ((stat ($stringsFile))[7] > 2) {		# check file size
				open (IFILE, "<:encoding(UTF-16LE)", $stringsFile) or die "Can't open file: $!";
				
				read (IFILE, $buf, 1);		# skip byte order mark
				if ($buf ne "\x{feff}") {
					print OFILE $buf;
				}
				
				my $content = <IFILE>;
				close (IFILE) or die "Can't close file: $!";
				
				print OFILE $content;
			}
		} else {
			if ($file =~ /.*\.rsrd$/) {
				printf "\t\t$file\n";
				copy (catfile ($roFolder, $file), catfile ($resTarget, $localResources));
			}
		}
	}
	close (OFILE) or die "Can't close $locStrings: $!";
}


#-----------------------------------------------------------------------
# Copy nonlocalisable resource files

if ($os eq "Mac") {
	
	$roFolder = catfile ($projectPath, $output, $fixed, $platform);
	
	my $localResources = $ENV{LANGUAGE_NAME} . ".lproj";
	system ("mkdir -p \"" . catfile ($resTarget, $localResources) . "\"") ;
	printf "\t=> Fixed resources\n";
	opendir (DIR, $roFolder);
	@files = readdir (DIR);
	closedir (DIR);
	foreach $file (@files) {
		if ($file =~ /.*\.rsrd$/) {
			printf "\t\t$file\n";
			copy (catfile ($roFolder, $file), catfile ($resTarget, $localResources));
		}
	}
}

print "done.\n";
