file(GLOB components_hpp *.hpp)
file(WRITE components.h "#include \"components_edit.h\"
")
foreach(cp_ele ${components_hpp})
    message("cp=${cp_ele}")
    string(REGEX MATCH "(.*/)(.*)" STRING_A_MATCHES ${cp_ele})
    set(DESIRED_SUBSTRING ${CMAKE_MATCH_2})
    file(APPEND components.h "#include \"${DESIRED_SUBSTRING}\" \n")
endforeach()