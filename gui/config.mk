CONF_NLS=1
CFLAGS=-std=c99 -fPIC -shared -Wall -g -DMAEMO -DMAEMO_VERSION=5 -Os
LDFLAGS=-export-dynamic -shared -Wl,-z,defs -Wl,-O1
MAEMO_VERSION=5
GAME_VERSION=1.3.5
