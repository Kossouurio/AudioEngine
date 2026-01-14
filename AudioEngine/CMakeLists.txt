# CMakeList.txt : projet CMake pour AudioEngine, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (AudioEngine "AudioEngine.cpp"  "Waveform_Parser.h" "Waveform_Parser.cpp" "AudioPlayer.h" "AudioPlayer.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AudioEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
