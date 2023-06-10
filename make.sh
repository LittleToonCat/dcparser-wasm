emcc -sEXPORTED_FUNCTIONS=FS,NODEFS -sWASM_BIGINT --closure 0 -lembind -lnodefs.js --post-js=post_js.js -o dcparser.js src/dcparser.cxx
