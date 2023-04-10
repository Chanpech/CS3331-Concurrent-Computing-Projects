
${EXE_FILE}: ${OBJ_FILE}
${CC} ${FLAGS} -o ${EXE_FILE} ${OBJ_FILE} ${TMLIB} -lpthread
quicksort.o: quicksort.cpp
${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c quicksort.cpp
quicksort-main.o: quicksort-main.cpp
${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c quicksort-main.cpp
noVisual: ${OBJ_FILE}
${CC} ${FLAGS} -o ${EXE_FILE} ${OBJ_FILE} ${TMLIB_NV} -lpthread
clean:
rm -f ${OBJ_FILE} ${EXE_FILE}