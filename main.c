#include <stdio.h>
#include <unistd.h>
#include <mikmod.h>
#include <python3.9/Python.h>

int main() {

	/* clear the screen and display the text */
	printf("\033[H\033[J");
	printf("== PUSH SIXEL ONTO MODERN TERMINALS!!! ==");
	printf("\nMusic: Michiel van den Bos - Foregone Destruction (Unreal Tournament Soundtrack)");

	/* load the sixel python script */
	char filename[] = "sixel.py";
	FILE* fp;

	Py_Initialize();
	fp = _Py_fopen(filename, "r");
	PyRun_SimpleFile(fp, filename);
	Py_Finalize();

	/* play the music */
	MODULE *module;
        MikMod_RegisterAllDrivers();
        MikMod_RegisterAllLoaders();
     
        md_mode |= DMODE_SOFT_MUSIC;
        if (MikMod_Init("")) {
            fprintf(stderr, "Could not initialize sound, reason: %s\n",
                    MikMod_strerror(MikMod_errno));
            return 1;
        }
     
        /* load module */
        module = Player_Load("foregone.it", 64, 0);
        if (module) {
            /* start module */
            Player_Start(module);
     
            while (Player_Active()) {
                /* we're playing */
                usleep(10000);
                MikMod_Update();
            }
     
            Player_Stop();
            Player_Free(module);
        } else
            fprintf(stderr, "Could not load module, reason: %s\n",
                    MikMod_strerror(MikMod_errno));
     
        /* give up */
        MikMod_Exit();
	return 0;
}
