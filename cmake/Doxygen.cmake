function(enable_doxygen)
  option(ENABLE_DOXYGEN "Enable doxygen doc builds of source" OFF)

  if(ENABLE_DOXYGEN)
    find_package(Doxygen)

    if (Doxygen_FOUND)
      set(DOXYGEN_EXCLUDE_PATTERNS
            */node_modules/*
            */viennacl/*
            */_build/*
      )  
      set(DOXYGEN_CALLER_GRAPH ON)
      set(DOXYGEN_CALL_GRAPH ON)
      set(DOXYGEN_EXTRACT_ALL ON)
      set(DOXYGEN_SOURCE_BROWSER YES)
      set(DOXYGEN_EXTRACT_PRIVATE YES)
      set(DOXYGEN_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/api-docs")    

      # make api-docs
      doxygen_add_docs(api-docs ${PROJECT_SOURCE_DIR} ALL)
    else()
      message(WARNING "doxygen not found")
    endif()
  endif()
endfunction()
