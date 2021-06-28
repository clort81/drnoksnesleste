2021-06-28

High time to share the WIP, since most things workforme.

libxsp and libsdl-haa (hildon animation actor) are temporarily included in subdirs,
as these are not available in leste repos at this time.

This doesn't yield the best build I ever had - a few games do not run, and 
fullscreen toggle isn't working, though it was once.

The hildon animation actor modes also need work, possibly due to being written for
16-bpp color depth.

If you have the build dependencies, it should build with "dpkg-buildpackage -rfakeroot -b -uc -us"

The application icon doesn't appear until restart - there may be a way to trigger a
rescan in postinst.

-clort
