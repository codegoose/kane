## kane

<p align="center">
 <img src="https://github.com/codegoose/kane/blob/master/snapshots/0.png?raw=true">
</p>

### Asset compilation

```
cd emico
git clone <redacted> assets
rm -r -force extra
rm -r -force .git
python emico.py
```
Output:
```
asset category root -> assets/fonts fonts
transcribe -> (assets/fonts/comfortaa.ttf) comfortaa.ttf -> fonts
transcribe -> (assets/fonts/comfortaa_bold.ttf) comfortaa_bold.ttf -> fonts
transcribe -> (assets/fonts/comfortaa_light.ttf) comfortaa_light.ttf -> fonts
transcribe -> (assets/fonts/nectar.ttf) nectar.ttf -> fonts
asset category root -> assets/music music
transcribe -> (assets/music/black_energy.ogg) black_energy.ogg -> music
...
68 sources to list.
```
### App compilation
```
cd ..
mkdir build
cd build
cmake -G Ninja .. -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld
ninja
```
