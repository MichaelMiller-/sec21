if (WIN32)
   set(WIN_CODE_SIGN_PFX "" CACHE STRING "Name of the Personal Information Exchange (PFX) file")
   set(WIN_CODE_SIGN_PFX_PASSWORD "" CACHE STRING "Optional password of the PFX file")
endif()

function(configure_sign_params)
   if (NOT SIGN_PARAMS)

   if (WIN_CODE_SIGN_PFX_PASSWORD) 
      set(PASSWORD_ARGUMENT /p ${WIN_CODE_SIGN_PFX_PASSWORD})
   endif()

   set(SIGN_PARAMS 
      /fd SHA256 
      /f ${WIN_CODE_SIGN_PFX} 
      /t http://timestamp.digicert.com 
      ${PASSWORD_ARGUMENT}
      PARENT_SCOPE)
   endif()
endfunction()

function(sign target_name)
   if (WIN32 AND WIN_CODE_SIGN_PFX)
      find_package(signtool REQUIRED)

      if (signtool_EXECUTABLE)
         configure_sign_params()
    
         add_custom_command(TARGET ${target_name}
            POST_BUILD
            COMMAND ${signtool_EXECUTABLE} sign ${SIGN_PARAMS} $<TARGET_FILE:${target_name}>
            VERBATIM)
      endif()
   endif()
endfunction()
