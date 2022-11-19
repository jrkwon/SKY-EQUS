#include "..\Qualcomm\comdef.h"             /* Definitions for byte, word, etc.     */







/* Melody data  */
/*extern const word aior1[];
extern const word aior2[];
extern const word akahan1[];
extern const word akahan2[];
extern const word alone1[];
extern const word alone2[];
extern const word ame[];
extern const word anpan[];
extern const word aria1[];
extern const word aria2[];
extern const word B_tango[];
extern const word Bicycle[];
extern const word bonodori[];
extern const word carmen[];
extern const word catch1[];
extern const word catch2[];
extern const word catch3[];
extern const word catch4[];
extern const word catch5[];
extern const word catch6[];
extern const word catch7[];
extern const word catch8[];
extern const word catch9[];
extern const word catch10[];
extern const word catch11[];
extern const word catch12[];
extern const word catch13[];
extern const word catch14[];
extern const word charumer[];
extern const word chime[];
extern const word cimu1[];
extern const word cimu2[];
extern const word Craction[];
extern const word Dango3[];
extern const word Denwa1[];
extern const word Denwa2[];
extern const word Denwa3[];
extern const word Denwa4[];
extern const word Denwa5[];
extern const word Denwa6[];
extern const word Denwa7[];
extern const word Denwa8[];
extern const word Denwa9[];
extern const word Hageyama[];
extern const word Hallelus[];
extern const word Hiirag1[];
extern const word Hiirag2[];
extern const word Hoshi[];
extern const word Humikiri[];
extern const word Jing1[];
extern const word Jing2[];
extern const word Kakko[];
extern const word Kekkon1[];
extern const word Kekkon2[];
extern const word Kitayado[];
extern const word Koorogi[];
extern const word Kozo1[];
extern const word Kozo2[];
extern const word Kuroden[];
extern const word Lastw1[];
extern const word Lastw2[];
extern const word Melody[];
extern const word Menuet[];
extern const word Miche1[];
extern const word Miche2[];
extern const word Movin_o2[];
extern const word Nagai[];
extern const word Oncemo[];
extern const word Osirase[];
extern const word Scab[];
extern const word Sinsekai[];
extern const word Sl[];
extern const word Spring[];
extern const word Timegoes[];
extern const word Tomor[];
extern const word Tukutuku[];
extern const word Turkish[];
extern const word Valkyrie[];
extern const word Wave[];
extern const word White[];
extern const word Zenmai[];*/

/* Melody data pointer */
extern const snd_sound_type *melody_fn_p[];

typedef struct {
	byte	wave;	// 
	byte	multi;
	byte	tl;
	byte	fl;
	byte	vib;
	byte	egt;
	byte	ar;
	byte	dr;
	byte	sl;
	byte	rr;
	byte	sus;
} basic_Timbre_data;


typedef struct {
	basic_Timbre_data	Mod_T;
	basic_Timbre_data	Car_T;
} Timbre_data_type;


extern const Timbre_data_type GMIDI_timbre[128];

extern mus_compact_melody_type	mb_MyBellPreBuffer_music;
#ifdef FEATURE_MUSIC_BELL
extern mus_compact_melody_type	mb_MusicBellPreBuffer_music;
#endif// FEATURE_MUSIC_BELL