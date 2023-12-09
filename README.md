Sur Linux: 

--> Vous pouvez simplement faire cette ccmd git avec l’url  créer un repertoire de préference : 

 git clone https://github.com/gilcedr/simple-chat-with-socket-C 

==============================================================================================================================

--> Attention à la compilation: j’inclus un fichier C appellé design_gilCedrickCl.c 

Et un design_...S donc à la compilation il faut le joindre exemple réutilisable pour le compiler: 

@server 

gcc serverUdp.c design_gilCedrickS.c -o server 

./server 

@client  

 gcc clientUdp.c design_gilCedrickCl.c -o client 

./client 

==============================================================================================================================
