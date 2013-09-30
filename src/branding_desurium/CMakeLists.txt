ExternalProject_Add(
	update_branding
	DOWNLOAD_COMMAND ""
	UPDATE_COMMAND ""
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
)
	
add_custom_command(TARGET update_branding POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}\\data" "${CMAKE_OUTPUT}\\data")

macro(RenameFile org dest path)
	add_custom_command(TARGET update_branding POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_OUTPUT}\\data\\themes\\default\\${path}\\${org}" "${CMAKE_OUTPUT}\\data\\themes\\default\\${path}\\${dest}")
endmacro()

if(WIN32)
  RenameFile(menu_button_hover_win.png menu_button_hover.png "images\\app")
  RenameFile(menu_button_nonactive_win.png menu_button_nonactive.png "images\\app")
  RenameFile(menu_button_win.png menu_button.png "images\\app")
  
  RenameFile(functions_win.js functions.js "js")
  RenameFile(settings_win.js settings.js "js") 
else()
  RenameFile(menu_button_hover_lin.png menu_button_hover.png "images\\app")
  RenameFile(menu_button_nonactive_lin.png menu_button_nonactive.png "images\\app")
  RenameFile(menu_button_lin.png menu_button.png "images\\app")
  
  RenameFile(functions_lin.js functions.js "js")
  RenameFile(settings_lin.js settings.js "js")  
endif()

SET_PROPERTY(TARGET update_branding PROPERTY FOLDER "Common")
