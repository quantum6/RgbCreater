
EXE_FILE=RgbToFile
if [ -f ${EXE_FILE} ]; then
    rm ${EXE_FILE}
fi

gcc \
    ../YuvLib/YuvLibFile.c ../YuvLib/YuvLibRgb.c \
    RgbToFileApp.c \
    -o ${EXE_FILE} \
    -I ../YuvLib \
    -lm \

./${EXE_FILE}

