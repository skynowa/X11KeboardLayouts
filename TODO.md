# TODO

Next ID: QLG-006

- [+] As single appication
- [ ] QLG-005 Detect lang code by xkbEvent->state.group
- [+] QLG-001 The app fails to load its bundled icons. [Config.h (line 22)](/home/skynowa/Projects/Qt/X11KeyboardLayouts/Config.h:22) points to /home/skynowa/Projects/X11KeboardLayouts/Data, which does not exist. The SVG files are in this repository’s Data/ directory. Use Qt resources or a path resolved from the executable.
- [+] QLG-002 Startup triggers an X11 BadAccess error on this display. [main.cpp (line 68)](/home/skynowa/Projects/Qt/X11KeyboardLayouts/main.cpp:68) selects ButtonPressMask on the root window. The runtime smoke test logged BadAccess, and the custom error handler then treated it as handled. Remove that selection if button events are unnecessary; otherwise handle failure explicitly.
- [ ] QLG-003 Keyboard changes can be missed. [main.cpp (line 91)](/home/skynowa/Projects/Qt/X11KeyboardLayouts/main.cpp:91) creates a QApplication and runs its event loop for 700 ms after each XKB event. During that time, the outer XNextEvent loop is paused. Keep one application event loop running and integrate X11 events into it.
- [ ] QLG-004 Layout detection assumes three fixed group numbers. [Widget.cpp (line 26)](/home/skynowa/Projects/Qt/X11KeyboardLayouts/Widget.cpp:26) maps groups 0–2 to en, ru, and ua. Other keyboard configurations produce an empty code and a missing icon. The project’s [TODO.md (line 4)](/home/skynowa/Projects/Qt/X11KeyboardLayouts/TODO.md:4) also identifies this gap.
