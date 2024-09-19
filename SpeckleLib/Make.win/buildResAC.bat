REM AC Resource build script

ECHO "Building AC Resources"

if not exist "%RES_OUTPUT%" MD "%RES_OUTPUT%"
python "%HEADER_PATH_5%\Tools\CompileResources.py" "INT" "%HEADER_PATH_5%\.." "%HEADER_PATH_6%\Connector" "%SYMROOT%\" "%RES_OUTPUT%" "%RES_SOURCE%\Speckle Connector.apx.mui"
python "%HEADER_PATH_5%\Tools\CompileResources.py" "INT" "%HEADER_PATH_5%\.." "%HEADER_PATH_4%\Speckle" "%HEADER_PATH_4%" "%RES_OUTPUT%" "%RES_SOURCE%\Speckle Connector.apx.mui"

ECHO "Finished AC Resource"
