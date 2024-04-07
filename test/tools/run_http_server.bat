@echo on

:: Check if two arguments are provided
IF "%~2"=="" (
    echo Usage: %0 "C:\path\to\http-server\main.py" "<socket-port>"
    exit /b 1
)

:: Store the arguments in variables
SET "PYTHON_SCRIPT=%~1"
SET "SOCKET_PORT=%~2"

echo Activating virtual environment...
call "%TEMP%\.venv\Scripts\activate.bat"

echo Running Python script...
echo Command: python "%PYTHON_SCRIPT%" --socket-port "%SOCKET_PORT%"

:: Run the Python script with the socket port argument
python "%PYTHON_SCRIPT%" --socket-port "%SOCKET_PORT%"
IF %ERRORLEVEL% NEQ 0 echo Python script execution failed & exit /b %ERRORLEVEL%

echo Script finished successfully.
