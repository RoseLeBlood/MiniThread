#!/bin/zsh
mkdir -p mini_thread/inc &> /dev/null
cp -r ../include/* ./mini_thread/inc/

rm -r mini_thread2.0.4.tar.zst &> /dev/null
tar cfv mini_thread2.0.4.tar.zst  mini_thread/ &> /dev/null
mv mini_thread2.0.4.tar.zst  ../mini_thread2.0.4.tar.zst 

rm -r mini_thread/inc &> /dev/null

echo "ready"

