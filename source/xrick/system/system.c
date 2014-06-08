/*
 * xrick/src/system.c
 *
 * Copyright (C) 1998-2002 BigOrno (bigorno@bigorno.net). All rights reserved.
 *
 * The use and distribution terms for this software are contained in the file
 * named README, which can be found in the root of this distribution. By
 * using this software in any fashion, you are agreeing to be bound by the
 * terms of this license.
 *
 * You must not remove this notice, or any other, from this software.
 */

#include <SDL.h>

#include <stdarg.h>   /* args for sys_panic */
#include <fcntl.h>    /* fcntl in sys_panic */
#include <stdio.h>    /* printf */
#include <stdlib.h>
#include <string.h>   /* strlen */
#include <signal.h>

#include "xrick/system/system.h"
#include "xrick/config.h"
#ifdef ENABLE_SOUND
#include "xrick/system/syssnd.h"
#endif

/*
 * Panic
 */
void
sys_panic(const char *err, ...)
{
  va_list argptr;
  char s[1024];

  /* change stdin to non blocking */
  /*fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);*/
  /* NOTE HPUX: use ... is it OK on Linux ? */
  /* fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~O_NDELAY); */

  /* prepare message */
  va_start(argptr, err);
  vsprintf(s, err, argptr);
  va_end(argptr);

  /* print message and die */
  printf("%s\npanic!\n", s);
  exit(1);
}

/*
 * Print a message to standard output
 */
void
sys_printf(const char *msg, ...)
{
  va_list argptr;
  char s[1024];

  /* change stdin to non blocking */
  /*fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);*/
  /* NOTE HPUX: use ... is it OK on Linux ? */
  /* fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~O_NDELAY); */

  /* prepare message */
  va_start(argptr, msg);
  vsprintf(s, msg, argptr);
  va_end(argptr);
  printf(s);
}

/*
 * Print a message to string buffer
 */
void
sys_snprintf(char *buf, size_t size, const char *msg, ...)
{
    va_list argptr;

    va_start(argptr, msg);
    vsnprintf(buf, size, msg, argptr);
    va_end(argptr);
}

/*
 * Returns string length
 */
size_t
sys_strlen(const char * str)
{
    return strlen(str);
}

/*
 * Return number of milliseconds elapsed since first call
 */
U32
sys_gettime(void)
{
    return SDL_GetTicks();
}

/*
 * Sleep a number of milliseconds
 */
void
sys_sleep(U32 ms)
{
    SDL_Delay(ms);
}

/*
 * Initialize system
 */
bool
sys_init(int argc, const char **argv)
{
	if (!sysarg_init(argc, argv))
    {
        return false;
    }
    if (!sysmem_init())
    {
        return false;
    }
	if (!sysvid_init())
    {
        return false;
    }
#ifdef ENABLE_JOYSTICK
	if (!sysjoy_init())
    {
        return false;
    }
#endif
#ifdef ENABLE_SOUND
	if (!sysarg_args_nosound && !syssnd_init())
    {
        return false;
    }
#endif
    if (!sysfile_setRootPath(sysarg_args_data? sysarg_args_data : sysfile_defaultPath))
    {
        return false;
    }
	atexit(sys_shutdown);
	signal(SIGINT, exit);
	signal(SIGTERM, exit);
    return true;
}

/*
 * Shutdown system
 */
void
sys_shutdown(void)
{
	sysfile_clearRootPath();
#ifdef ENABLE_SOUND
	syssnd_shutdown();
#endif
#ifdef ENABLE_JOYSTICK
	sysjoy_shutdown();
#endif
	sysvid_shutdown();
    sysmem_shutdown();
}

/*
 * Preload data before entering main loop
 */
bool
sys_cacheData(void)
{
#ifdef ENABLE_SOUND
	/*  tune[0-5].wav not cached */
    soundGameover->dispose = false;
	soundSbonus2->dispose = false;
	soundBullet->dispose = false;
	soundBombshht->dispose = false;
	soundExplode->dispose = false;
	soundStick->dispose = false;
	soundWalk->dispose = false;
	soundCrawl->dispose = false;
	soundJump->dispose = false;
	soundPad->dispose = false;
	soundBox->dispose = false;
	soundBonus->dispose = false;
	soundSbonus1->dispose = false;
	soundDie->dispose = false;
	soundEntity[0]->dispose = false;
	soundEntity[1]->dispose = false;
	soundEntity[2]->dispose = false;
	soundEntity[3]->dispose = false;
	soundEntity[4]->dispose = false;
	soundEntity[5]->dispose = false;
	soundEntity[6]->dispose = false;
	soundEntity[7]->dispose = false;
	soundEntity[8]->dispose = false;
	syssnd_load(soundGameover);
	syssnd_load(soundSbonus2);
	syssnd_load(soundBullet);
	syssnd_load(soundBombshht);
	syssnd_load(soundExplode);
	syssnd_load(soundStick);
	syssnd_load(soundWalk);
	syssnd_load(soundCrawl);
	syssnd_load(soundJump);
	syssnd_load(soundPad);
	syssnd_load(soundBox);
	syssnd_load(soundBonus);
	syssnd_load(soundSbonus1);
	syssnd_load(soundDie);
	syssnd_load(soundEntity[0]);
	syssnd_load(soundEntity[1]);
	syssnd_load(soundEntity[2]);
	syssnd_load(soundEntity[3]);
	syssnd_load(soundEntity[4]);
	syssnd_load(soundEntity[5]);
	syssnd_load(soundEntity[6]);
	syssnd_load(soundEntity[7]);
	syssnd_load(soundEntity[8]);
#endif
    return true;
}

/*
 * Clear preloaded data before shutdown
 */
void
sys_uncacheData(void)
{
#ifdef ENABLE_SOUND
    syssnd_free(soundGameover);
	syssnd_free(soundSbonus2);
	syssnd_free(soundBullet);
	syssnd_free(soundBombshht);
	syssnd_free(soundExplode);
	syssnd_free(soundStick);
	syssnd_free(soundWalk);
	syssnd_free(soundCrawl);
	syssnd_free(soundJump);
	syssnd_free(soundPad);
	syssnd_free(soundBox);
	syssnd_free(soundBonus);
	syssnd_free(soundSbonus1);
	syssnd_free(soundDie);
	syssnd_free(soundEntity[0]);
	syssnd_free(soundEntity[1]);
	syssnd_free(soundEntity[2]);
	syssnd_free(soundEntity[3]);
	syssnd_free(soundEntity[4]);
	syssnd_free(soundEntity[5]);
	syssnd_free(soundEntity[6]);
	syssnd_free(soundEntity[7]);
	syssnd_free(soundEntity[8]);
#endif
}

/* eof */
