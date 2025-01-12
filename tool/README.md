# What's this?

My primary desktop is an M1 mac.  I needed a way to cross-compile to 
amd64 to test the code.  To do so, I created a simple docker image
that has the necessary tools to build on this platform.

Once built, the kernel and loader can be run using qemu on the mac.

## Building

```
docker build . -t xbuilder
```

## Usage

I created the xbuilder script (also contained here) to abstract away
the command line.  When you're ready to build, use the script
`xbuilder` instead of `make`.  I put the script in my path for 
convenience.