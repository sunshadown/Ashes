@echo OFF

set DATA_DIR=.\

setlocal disableDelayedExpansion
for /f "delims=" %%A in ('forfiles /s /p %DATA_DIR% /m Test-*.exe /c "cmd /c echo @relpath"') do (
	set "file=%%~A"
	setlocal enableDelayedExpansion
	echo "Testing !file:~2!"
	%DATA_DIR%\!file:~2!
	%DATA_DIR%\!file:~2! -gl4
	%DATA_DIR%\!file:~2! -gl3
	%DATA_DIR%\!file:~2! -d3d11
	endlocal
)

setlocal disableDelayedExpansion
for /f "delims=" %%A in ('forfiles /s /p %DATA_DIR% /m Sample-*.exe /c "cmd /c echo @relpath"') do (
	set "file=%%~A"
	setlocal enableDelayedExpansion
	echo "Testing !file:~2!"
	%DATA_DIR%\!file:~2!
	%DATA_DIR%\!file:~2! -gl4
	%DATA_DIR%\!file:~2! -gl3
	%DATA_DIR%\!file:~2! -d3d11
	endlocal
)
