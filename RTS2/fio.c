#include "fio.h"

#define clen 25

gBool saveSet()
{
	FILE *settings;

	settings = fopen("config/settings2.txt","w");
	if(settings == NULL)
	{
		fprintf(stderr,"Save operation failed, file could not be created.\n");
		return false;
	}
	else
		fprintf(stderr,"Saving settings...");

	// Sound
	fprintf(settings, "sound\n");
	fprintf(settings, "fxvolume %f\n", audio.eVol);
	fprintf(settings, "musicvolume %f\n", audio.mVol);
	fprintf(settings, "mute %i\n\n", audio.mute);

	// Controls

	// HUD
	fprintf(settings,"HUD\n");
	fprintf(settings,"minimap %i %i %i %i\n",HUD.mapx,HUD.mapy,HUD.mapw,HUD.maph);
	fprintf(settings,"infobar %i %i %i %i\n",HUD.infox,HUD.infoy,HUD.infow,HUD.infoh);
	fprintf(settings,"player %i %i %i %i\n",HUD.playerx,HUD.playery,HUD.playerw,HUD.playerh);
	fprintf(settings,"alpha %i\n\n",aLevel);

	// Game
	fprintf(settings, "game\n");
	fprintf(settings, "scrollspeed %f\n", sSpeed);
	fprintf(settings, "tooltips %i\n", HUD.ttips);
	fprintf(settings, "resolution %i %i\n\n", HUD.res.w, HUD.res.h);

	fprintf(stderr,"done\n\n");
	fclose(settings);
	return true;
}

gBool loadSet()
{
	FILE *settings;		// pointer to settings file
	char cmd[clen];		// command string
	char cmd2[clen];	// 2nd command string
	Uint8 panel = 0;	// which panel the command applies to
	float val = 1;		// temp values

	fprintf(stderr,"Loading settings...");

	settings = fopen("config/settings.txt","r");
	if(settings == NULL)
	{
		fprintf(stderr,"failed.\n");
		return false;
	}
	else
		fprintf(stderr,"\n");

	// Check each line for section name and then for individual settings.
	while(fgets(cmd,clen,settings) != NULL)
	{
		trim(cmd);
		fprintf(stderr,"- %s\n",cmd);

		// sound
		if(m_strncmp(cmd, "sound", clen) )
		{
			while( fgets(cmd,clen,settings) != NULL && !m_strncmp(cmd,"\n",clen) )
			{
				trim(cmd);

				if(sscanf(cmd,"%s%*c%f%*c",cmd,&val) != 2)
				{
					fprintf(stderr," Error reading audio settings.\n");
					continue;
				}

				if(m_strncmp(cmd, "fxvolume", clen) )
				{
					audio.eVol = val;
				}
				else if(m_strncmp(cmd, "musicvolume", clen) )
				{
					audio.mVol = val;
				}
				else if(m_strncmp(cmd, "mute", clen) )
				{
					audio.mute = (int)val;
				}
			}
		}
/*
		// ADD controls
		else if(m_strncmp(cmd, "controls", clen) )
		{
			while(fgets(cmd,clen,settings) != EOF && !m_strncmp(cmd,"\n",clen) )
			{
				
			}
		}
*/
		// HUD
		else if( m_strncmp(cmd, "HUD", clen) )
		{
			while( fgets(cmd, clen, settings) != NULL && !m_strncmp(cmd,"\n",clen) )
			{
				trim(cmd);
				//fprintf(stderr,"cmd: %s\n",cmd);

				if( sscanf(cmd,"%s",cmd2) != 1)
					fprintf(stderr," Error reading panel settings.\n");
				else
					fprintf(stderr," - %s\n",cmd2);

				if( m_strncmp(cmd2, "minimap", clen) )
				{
					sscanf(cmd,"%*s%*c%i%i%i%i",&HUD.mapx,&HUD.mapy,&HUD.mapw,&HUD.maph);
				}
				else if(m_strncmp(cmd2, "infobar", clen))
				{
					sscanf(cmd,"%*s%*c%i%i%i%i",&HUD.infox,&HUD.infoy,&HUD.infow,&HUD.infoh);
				}
				else if(m_strncmp(cmd2, "player", clen))
				{
					sscanf(cmd,"%*s%*c%i%i%i%i",&HUD.playerx,&HUD.playery,&HUD.playerw,&HUD.playerh);
				}
				else if(m_strncmp(cmd2, "alpha", clen))
				{
					sscanf(cmd,"%*s%*c%f",&aLevel);
				}
			}
		}

		// game
		else if( m_strncmp(cmd, "game", clen) )
		{
			while(fgets(cmd,clen,settings) != NULL && !m_strncmp(cmd,"\n",clen) )
			{
				trim(cmd);
				sscanf(cmd,"%s%*c%f",cmd,&val);

				if(m_strncmp(cmd, "scrollspeed", clen))
					sSpeed = val;

				if(m_strncmp(cmd, "tooltips", clen))
					HUD.ttips = val;

				if(m_strncmp(cmd, "resolution", clen))
					sscanf(cmd,"%*s%*c%i%i",HUD.res.w,HUD.res.h);
			}
		}
	}

	fprintf(stderr,"...done\n\n");
	fclose(settings);
	return true;
}
