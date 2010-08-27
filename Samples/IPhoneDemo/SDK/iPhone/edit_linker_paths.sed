s/\/Debug\/\([a-zA-Z0-9_]*\.a\)/\/$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME)\/\1/g
s/\/Release\/\([a-zA-Z0-9_]*\.a\)/\/$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME)\/\1/g
s/\/RelWithDebInfo\/\([a-zA-Z0-9_]*\.a\)/\/$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME)\/\1/g
s/\/MinSizeRel\/\([a-zA-Z0-9_]*\.a\)/\/$(CONFIGURATION)$(EFFECTIVE_PLATFORM_NAME)\/\1/g
