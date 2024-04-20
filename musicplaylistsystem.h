#pragma once
#pragma once
#ifndef PLAYLIST_H
#define PLAYLIST_H

typedef struct Song
{
    char title[100];
    char artist[100];
    int duration;
    struct Song* next;
} Song;

typedef struct Playlist
{
    char name[100];
    Song* head;
    struct Playlist* next;
} Playlist;

#define HASH_SIZE 100
typedef struct HashTable
{
    Playlist* table[HASH_SIZE];
} HashTable;

unsigned int hash(const char* key);
void insertPlaylist(HashTable* table, Playlist* playlist);
Playlist* searchPlaylist(HashTable* table, const char* name);
void searchPlaylistsBySong(HashTable* table, const char* songTitle);
Song* createSong(const char* title, const char* artist, int duration);
void addSong(Playlist* playlist, const char* title, const char* artist, int duration);
void displayPlaylist(Playlist* playlist);

#endif