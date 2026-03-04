{
  cmake,
  extra-cmake-modules,
  hyprlandPlugins,
  kdePackages,
  lib,
  libevdev,
  pkg-config,
  yaml-cpp,
  ...
}:

hyprlandPlugins.mkHyprlandPlugin rec {
  pluginName = "inputactions_hyprland";
  version = "0.9.0.0";

  src = ./..;

  dontWrapQtApps = true;

  nativeBuildInputs = [
    extra-cmake-modules
  ];

  buildInputs = [
    kdePackages.qtbase
    libevdev
    pkg-config
    yaml-cpp
  ];

  meta = with lib; {
    description = "Linux utility for binding keyboard. mouse, touchpad and touchscreen actions to system actions (Hyprland compositor plugin implementation)";
    license = licenses.gpl3;
    homepage = "https://github.com/InputActions/hyprland";
  };
}
