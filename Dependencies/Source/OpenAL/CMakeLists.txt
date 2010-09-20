add_library(OpenAL OpenALWrap.c)

config_target_if_iphone_build(OpenAL TRUE)
