RELATIVE_CWD_SCRIPT_PATH=$0
ABSOLUTE_SCRIPT_PATH=$(realpath $RELATIVE_CWD_SCRIPT_PATH)
ABSOLUTE_SCRIPTS_DIRECTORY_PATH=$(dirname $ABSOLUTE_SCRIPT_PATH)
ABSOLUTE_SCRIPT_GLOBALS_ENV_PATH="$ABSOLUTE_SCRIPTS_DIRECTORY_PATH/SCRIPT_GLOBALS.env"
source $ABSOLUTE_SCRIPT_GLOBALS_ENV_PATH

if [ -d "$OUTPUT_DIRECTORY_PATH" ]; then
    rm -rf $OUTPUT_DIRECTORY_PATH
fi
