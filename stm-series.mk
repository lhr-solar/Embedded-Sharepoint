# This Makefile is used to determine the series and line of a STM32 MCU based on the target name.
# The target name is expected to be in the format "stm32<series><line><extra>".
# The Makefile will output the following variables:
# - SERIES: the series of the MCU
# - LINE: the line of the MCU
# - EXTRA: the extra characters of the MCU
# - SERIES_GENERIC: the generic series of the MCU
# - SERIES_GENERIC_CAP: the capitalized generic series of the MCU
# - SERIES_LINE: the series and line of the MCU
# - SERIES_LINE_CAP: the capitalized series and line of the MCU
# - SERIES_LINE_GENERIC: the generic series and line of the MCU
# - SERIES_LINE_GENERIC_CAP: the capitalized generic series and line of the MCU
# The Makefile will also output an error message if the generic series and line of the MCU is not found in the CMSIS directory.

SERIES := $(shell echo "$(PROJECT_TARGET)" | cut -c6-7)
LINE := $(shell echo "$(PROJECT_TARGET)" | cut -c8-9)
EXTRA := $(shell echo "$(PROJECT_TARGET)" | cut -c10-)
EXTRA_CUT := $(shell echo "$(PROJECT_TARGET)" | cut -c10-11)

SERIES_CAP := $(shell echo "$(SERIES)" | tr '[:lower:]' '[:upper:]')
EXTRA_CAP := $(shell echo "$(EXTRA)" | tr '[:lower:]' '[:upper:]')
SERIES_GENERIC := stm32$(SERIES)xx
SERIES_GENERIC_CAP := STM32$(SERIES_CAP)xx

SERIES_LINE := stm32$(SERIES)$(LINE)
SERIES_LINE_CAP := STM32$(SERIES_CAP)$(LINE)

MCU_MATCHES := $(shell ls stm/"$(SERIES_GENERIC)"/CMSIS/Device/ST/"$(SERIES_GENERIC_CAP)"/Include | sed 's/\.h//g' | sed 's/x/./g')

SERIES_LINE_GENERIC := $(shell \
	for match in $(MCU_MATCHES); do \
		if [ $${#match} -eq 11 ] && echo "stm32$(SERIES_LINE)$(EXTRA_CUT)" | grep -qE "$$match"; then \
			echo $$match | sed 's/\./x/g'; \
			break; \
		fi; \
	done)

SERIES_LINE_GENERIC_CAP := $(shell echo "$(SERIES_LINE_GENERIC)" | sed 's/[^x]/\U&/g')

ifeq ($(SERIES_LINE_GENERIC),)
$(error SERIES_LINE_GENERIC is not found in stm/$(SERIES_GENERIC)/CMSIS/Device/ST/$(SERIES_GENERIC_CAP)/Include. Please check the input)
endif

export SERIES
export LINE
export EXTRA
export EXTRA_CAP
export SERIES_GENERIC
export SERIES_GENERIC_CAP
export SERIES_LINE
export SERIES_LINE_CAP
export SERIES_LINE_GENERIC
export SERIES_LINE_GENERIC_CAP