SYSTEM_INCLUDES += $(LIB_DIR)/L0_Platform/arm_cortex/m4/ARM_CM4F/
COMMON_FLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
                -fabi-version=0 -mtpcs-frame -mtpcs-leaf-frame
