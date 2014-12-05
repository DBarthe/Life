Life
====

A game of life with SDL

![Alt text](./example.png "")
https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

Some features :
* zoom, camera moving
* significant cells colorization
* maps are loaded from images (see in maps/ for example)
* ascii mode
* fullscreen
* circular and closed maps
* clock is modifiable
* possibility to change the rules of the automaton statistics bar

Compilation
-----------
~~~bash
$ make

~~~

Usage
-----
~~~bash
$ ./life [ options ] map_file
~~~

####Options:
* __-x__ map_width

>set the map witdh (default: same as in the map file)

* __-y__ map_height (default: same as map file)

>set the map heigth (default: same as in the map file)

* __-t__ delay

>set the delay (in ms) bewteen each generation (def:30)

* __-m__

>switch to closed map mode (problem with camera movements)

* __-r__ a[/b]

>change the rules. For example: __-r 3/23__ means 3 neighbors to be born, 2 or 3 to survive, dead otherwise.

* __-f__

>display in full screen

* __-c__

>swith to ascii mode

####Examples:
~~~bash
$ ./life maps/double_breeder.png                  # classic
$./life maps/BigAnts.bmp  -x 1000 -y 1000 -r 1/2  # epileptic
~~~

Key Control
-----------
> R -> run

> P -> pause

> S -> stop

> A -> zoom+

> Z -> zoom-

> arrows -> move camera

> space -> reset the camera position

> C -> faster

> V -> slower

