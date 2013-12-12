-=== GT3 ===-

=== Gruppenmitglieder ===

Manuel Bellersen
Martin Stark
Francesco Tietke

=== Was ist drin ===

C++, OpenGL3, Bullet Physics (nicht animierten Meshes haben Physics, animierte siehe Kick-Teil), 
Grafik (Shader, Texturen, Licht, Debugansichten), Animation, Skelett, Hintergrundmusik, Sound, 
mehrere umschaltbare Szenen und Kameras (kamera mit physik), Kinect (OpenNI, NITE), shoot physics box

=== gesplittet in 3 Teile ===

Masterbranch (Hauptbranch, versch. Szenen (wechseln mit "n") mit Physics, Features siehe oben, Steuerung siehe unten)
Kick (Physics an animiertem bone "befestigt", der fallende Kisten wegkickt)
Grundarchitektur, Animation / Kinect, Processing (siehe SVN / Dropbox Link)

=== Hinweis ===

FPS Kamera besser zur ansicht (einschalten mit "v")

=== Steuerung ===

W - forward
S - backward
A - strafe left
D - strafe right

left Ctrl - fly down
Space - fly up
Q - roll left
E - roll right

C - change camera number
V - toggle camera FLY - FPS mode
P - toggle draw mode (POINT LINE FACE)
N - switch scene

F1 - toggle debug draw
F2 - toggle debug draw mode ( WIREFRAME FACE)
F3 - toggle ghost mode

Z - next song
U - previous song

Return - shoot box

Mouse move - rotate camera
Mouse press - shoot box

== Kinektsteuerung Masterbranch ==

Handbewegung zum Kalibrieren
Hand -> Maus (Bewegung links, rechts, hoch, runter)
Hand nach vorne -> Box schiessen