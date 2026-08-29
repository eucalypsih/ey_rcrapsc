install(
    TARGETS myp_exe
    RUNTIME COMPONENT myp_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
