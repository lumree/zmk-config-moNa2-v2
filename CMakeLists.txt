# Extra firmware helpers for moNa2 config

if(CONFIG_ZMK_INPUT_PROCESSOR_INERTIAL_SCROLL)
    target_sources(app PRIVATE src/input_processors/input_processor_inertial_scroll.c)
endif()
