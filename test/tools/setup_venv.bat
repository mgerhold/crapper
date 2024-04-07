@echo off

:: Check if a file path is provided as an argument
IF "%~1"=="" (
    echo Usage: %0 \path\to\requirements.txt
    exit /b 1
)

:: Store the requirements file path
SET REQUIREMENTS_FILE=%1

:: Check if the requirements file exists
IF NOT EXIST "%REQUIREMENTS_FILE%" (
    echo The specified requirements.txt file does not exist.
    exit /b 1
)

:: Delete the virtual environment if it exists
IF EXIST "%TEMP%\.venv\" (
    echo Deleting existing virtual environment at %TEMP%\.venv
    RMDIR /S /Q "%TEMP%\.venv"
)

:: Create a new virtual environment
echo Creating a new virtual environment at %TEMP%\.venv
python -m venv "%TEMP%\.venv"

:: Activate the virtual environment
call "%TEMP%\.venv\Scripts\activate"

:: Install dependencies from the requirements file
pip install -r "%REQUIREMENTS_FILE%"

echo Dependencies installed successfully.
