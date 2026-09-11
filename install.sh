#!/usr/bin/env bash

sudo pacman -Syu nlohmann-json

sudo mkdir /artex
cd /artex
sudo mkdir gitsave
sudo mkdir jsonsaves
sudo mkdir localfiles
sudo mkdir localsaves
sudo mkdir lastBackup
sudo touch versions.txt

cd gitsave
mkdir config

sudo git init
sudo git config --global user.email "ArtexRecoveryautogit@gmail.com"
sudo git config --global user.name "ArtexRecoveryautogit"

sudo cp ~/ArtexRecovery/basefiles/* /artex/localfiles

cd ~/ArtexRecovery

mkdir -p include/nlohmann
curl -L https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp -o include/nlohmann/json.hpp

g++ -std=c++17 main.cpp -I include -o ArtexRecovery
mv ArtexRecovery /artex/ArtexRecovery

sudo ln ~/ArtexRecovery/ArtexRecovery /usr/local/bin/ArtexRecovery