@ECHO OFF
rem -----------------------------------------------------------------------
rem This file is intended only to be used after the contents of the
rem intermediate directory have been created.  Do so by:
rem
rem     $ python build.py intermediate [skip_api]
rem
rem This is a modified version of the standard Sphinx `make.bat` file.
rem Changes:
rem   - uses these environment variables in the same way `build.py` does
rem     when they are set:
rem     - LVGL_DOC_BUILD_INTERMEDIATE_DIR
rem     - LVGL_DOC_BUILD_OUTPUT_DIR
rem   - Cleans up locally-created environment variables at end, so they
rem     do not clutter environment variables.
rem -----------------------------------------------------------------------

pushd %~dp0

REM Command file for Sphinx documentation

setlocal ENABLEDELAYEDEXPANSION

if "%SPHINXBUILD%" == "" (
	set SPHINXBUILD=sphinx-build
)
if "%LVGL_DOC_BUILD_INTERMEDIATE_DIR%" == "" (
	set SOURCEDIR=intermediate
) else (
	set SOURCEDIR=%LVGL_DOC_BUILD_INTERMEDIATE_DIR%
)
if "%LVGL_DOC_BUILD_OUTPUT_DIR%" == "" (
	set BUILDDIR=build
) else (
	set BUILDDIR=%LVGL_DOC_BUILD_OUTPUT_DIR%
)
if "%SPHINXOPTS%" == "" (
	rem python ./src/lvgl_version.py >_version_temp.txt
	rem set /p VER=<_version_temp.txt
	rem del _version_temp.txt
	for /F %%v in ('python lvgl_version.py') do set VER=%%v
	echo VERSION    [!VER!]
	set SPHINXOPTS=-D version="!VER!" -j 4
	set VER=
)

echo SOURCEDIR  [%SOURCEDIR%]
echo BUILDDIR   [%BUILDDIR%]
echo SPHINXOPTS [%SPHINXOPTS%]

%SPHINXBUILD% >NUL 2>NUL
if errorlevel 9009 (
	echo.
	echo.The 'sphinx-build' command was not found. Make sure you have Sphinx
	echo.installed, then set the SPHINXBUILD environment variable to point
	echo.to the full path of the 'sphinx-build' executable. Alternatively you
	echo.may add the Sphinx directory to PATH.
	echo.
	echo.If you don't have Sphinx installed, grab it from
	echo.https://www.sphinx-doc.org/
	exit /b 1
)

if "%1" == "" goto help

echo %SPHINXBUILD% -M %1 "%SOURCEDIR%" "%BUILDDIR%" %SPHINXOPTS% %2 %3 %4 %5 %6 %7 %8 %9
%SPHINXBUILD% -M %1 "%SOURCEDIR%" "%BUILDDIR%" %SPHINXOPTS% %2 %3 %4 %5 %6 %7 %8 %9
goto end

:help
%SPHINXBUILD% -M help "%SOURCEDIR%" "%BUILDDIR%" %SPHINXOPTS% %2 %3 %4 %5 %6 %7 %8 %9

:end
rem Clean up.
popd
set BUILDDIR=
set SOURCEDIR=
