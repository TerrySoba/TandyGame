# TandyGame

Supposed to be a small game to be run on a tandy 1000 computer.

## Building
The game is built using the open watcom c++ compiler.

If you have the open watcom compiler set up correctly the game can be built using the `Makefile` in the `source` directory.

```
cd source
wmake
```

## Building using Docker
If you do not have an installation of open watcom you may create a docker image using the file in the `open_watcom_docker` directory.

The two scripts `create_docker_image.sh` and `build.sh` may be used to first create a docker image and then build the game using the docker image. Depending on your docker installation you may have to be **root** to be able to do so:

```
sudo create_docker_image.sh
sudo build.sh
```

After this you should have a file called `game.exe` in the source directory.