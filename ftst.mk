# External programms
PYTHON				:= python3

# FTST test setup
FTST_SRCS			?=
FTST_DIR			?=	ftst-unit-test-framework/
FTST_TARGET			?=


FTST_TEST_GENERATOR	=	${FTST_DIR}gen_test_main.py
FTST_TEST_RUNNER_SRC =	ftst_test_runner.c

FTST_INC			+=	
FTST_LIB_DIRS		=
FTST_LIBS			=

FTST_SILENT_MODE	?=	1
FTST_SILENT			=	-D FTST_SILENT=${FTST_SILENT_MODE} 

FTST_MALLOC_MODE	?=	1
ifeq (${FTST_MALLOC_MODE}, 1)
FTST_MALLOC_TEST	=	-ldl -D FTST_MALLOC_TEST=${FTST_MALLOC_MODE}
else
FTST_MALLOC_TEST	=	-D FTST_MALLOC_TEST=${FTST_MALLOC_MODE}
endif

FTST_COLOR_MODE		?=	1
FTST_COLOR			=	-D FTST_COLOR=${FTST_COLOR_MODE}

TARGET_OS = ${shell uname}
ifeq (${TARGET_OS}, Linux)
FTST_ENTRY_POINT = -nostartfiles -Wl,-e_start
endif
ifeq (${TARGET_OS}, Darwin)
FTST_ENTRY_POINT = -nostartfiles -e__start
endif

FTST_FLAGS			= \
		${FTST_ENTRY_POINT} \
		${FTST_COLOR} \
		${FTST_MALLOC_TEST} \
		${FTST_SILENT} \
		${addprefix -L , ${FTST_LIB_DIRS}} \
		${addprefix -I , ${FTST_INC} ${FTST_DIR}include/}
FTST_EXE			=	ftst.out

.INTERMEDIATE: ${FTST_TEST_RUNNER_SRC}
${FTST_TEST_RUNNER_SRC}: ${FTST_SRCS}
			${PYTHON} ${FTST_TEST_GENERATOR} ${FTST_SRCS}

.INTERMEDIATE: ${FTST_EXE}
${FTST_EXE}:	${FTST_TEST_RUNNER_SRC} ${FTST_TARGET}
			${CC} ${FTST_FLAGS} ${FTST_SRCS} ${FTST_TEST_RUNNER_SRC} -o ${FTST_EXE} ${FTST_TARGET} ${FTST_LIBS}

test:		${FTST_EXE}
			./${FTST_EXE} && \
				printf ${PRETTY_STATUS}		"${PRETTY_DONE}" "${PRETTY_BUILD_NAME}" "${PRETTY_TARGET_NAME}" "${NAME}" "test" || \
			{	printf ${PRETTY_STATUS}		"${PRETTY_FAIL}" "${PRETTY_BUILD_NAME}" "${PRETTY_TARGET_NAME}" "${NAME}" "test" ; exit 1;	}

docs:
			${MAKE} doxygen -f docs/doxygen.mk
