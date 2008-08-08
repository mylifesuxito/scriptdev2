/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Boss_Mekgineer_Steamrigger
SD%Complete: 60
SDComment: Mechanics' interrrupt heal doesn't work very well, also a proper movement needs to be implemented -> summon further away and move towards target to repair.
SDCategory: Coilfang Resevoir, The Steamvault
EndScriptData */

/* ContentData
boss_mekgineer_steamrigger
mob_steamrigger_mechanic
EndContentData */

#include "def_steam_vault.h"

#define SAY_MECHANICS               "I'm bringin' the pain!"
#define SOUND_MECHANICS             10367

#define SAY_AGGRO_1                 "You're in for a world of hurt!"
#define SOUND_AGGRO_1               10368
#define SAY_AGGRO_2                 "Eat hot metal, scumbag!"
#define SOUND_AGGRO_2               10369
#define SAY_AGGRO_3                 "I'll come over there!"
#define SOUND_AGGRO_3               10370
#define SAY_AGGRO_4                 "I'm bringin' the pain!"
#define SOUND_AGGRO_4               10371

#define SAY_SLAY_1                  "You just got served, punk!"
#define SOUND_SLAY_1                10372
#define SAY_SLAY_2                  "I own you!"
#define SOUND_SLAY_2                10373
#define SAY_SLAY_3                  "Have fun dyin', cupcake!"
#define SOUND_SLAY_3                10374

#define SAY_DEATH                   "Mommy!"
#define SOUND_DEATH                 10375

#define SPELL_SUPER_SHRINK_RAY      31485
#define SPELL_SAW_BLADE             31486
#define SPELL_ELECTRIFIED_NET       35107
#define H_SPELL_ENRAGE              1                       //corrent enrage spell not known

#define ENTRY_STREAMRIGGER_MECHANIC 17951

struct MANGOS_DLL_DECL boss_mekgineer_steamriggerAI : public ScriptedAI
{
    boss_mekgineer_steamriggerAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 Shrink_Timer;
    uint32 Saw_Blade_Timer;
    uint32 Electrified_Net_Timer;
    bool Summon75;
    bool Summon50;
    bool Summon25;

    void Reset()
    {
       Shrink_Timer = 20000;
       Saw_Blade_Timer = 15000;
       Electrified_Net_Timer = 10000;

       Summon75 = false;
       Summon50 = false;
       Summon25 = false;
       
       if( pInstance ) pInstance->SetData(TYPE_MEKGINEER_STEAMRIGGER, NOT_STARTED);
    }

    void JustDied(Unit* Killer)
    { 
       DoYell(SAY_DEATH, LANG_UNIVERSAL, NULL);
       DoPlaySoundToSet(m_creature,SOUND_DEATH);

       if( pInstance ) pInstance->SetData(TYPE_MEKGINEER_STEAMRIGGER, DONE);
    }

    void KilledUnit(Unit* victim)
    {
        switch(rand()%3)
        {
        case 0:
            DoYell(SAY_SLAY_1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY_1);
            break;
        case 1:
            DoYell(SAY_SLAY_2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY_2);
            break;
        case 2:
            DoYell(SAY_SLAY_3, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_SLAY_3);
            break;
        }
    }

    void Aggro(Unit *who)
    {
        switch(rand()%3)
        {
        case 0:
            DoYell(SAY_AGGRO_1, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_AGGRO_1);
            break;
        case 1:
            DoYell(SAY_AGGRO_2, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_AGGRO_2);
            break;
        case 2:
            DoYell(SAY_AGGRO_3, LANG_UNIVERSAL, NULL);
            DoPlaySoundToSet(m_creature,SOUND_AGGRO_3);
            break; 
        }

        if( pInstance ) pInstance->SetData(TYPE_MEKGINEER_STEAMRIGGER, IN_PROGRESS);
    }

    //no known summon spells exist
    void SummonMechanichs()
    { 
        DoYell(SAY_MECHANICS, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature,SOUND_MECHANICS);

        DoSpawnCreature(ENTRY_STREAMRIGGER_MECHANIC,5,5,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 240000);
        DoSpawnCreature(ENTRY_STREAMRIGGER_MECHANIC,-5,5,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 240000);
        DoSpawnCreature(ENTRY_STREAMRIGGER_MECHANIC,-5,-5,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 240000);
        if( rand()%2 )
            DoSpawnCreature(ENTRY_STREAMRIGGER_MECHANIC,5,-7,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 240000);
        if( rand()%2 )
            DoSpawnCreature(ENTRY_STREAMRIGGER_MECHANIC,7,-5,0,0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 240000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if( Shrink_Timer < diff )
        { 
            DoCast(m_creature->getVictim(),SPELL_SUPER_SHRINK_RAY);
            Shrink_Timer = 20000;
       }else Shrink_Timer -= diff;

       if( Saw_Blade_Timer < diff )
       {
            if( Unit* target = SelectUnit(SELECT_TARGET_RANDOM,1) ) DoCast(target,SPELL_SAW_BLADE);
            else DoCast(m_creature->getVictim(),SPELL_SAW_BLADE);
            Saw_Blade_Timer = 15000;
        } else Saw_Blade_Timer -= diff;

        if( Electrified_Net_Timer < diff )
        {
            DoCast(m_creature->getVictim(),SPELL_ELECTRIFIED_NET);
            Electrified_Net_Timer = 10000;
        }
        else Electrified_Net_Timer -= diff;

        if( !Summon75 )
            if( (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 75 )
            {
                SummonMechanichs();
                Summon75 = true;
            }
        if( !Summon50 )
            if( (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50 )
            {
                SummonMechanichs();
                Summon50 = true;
            }
        if( !Summon25 )
            if( (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 25 )
            {
                SummonMechanichs();
                Summon25 = true;
            }

        DoMeleeAttackIfReady();
    } 
};

CreatureAI* GetAI_boss_mekgineer_steamrigger(Creature *_Creature)
{
    return new boss_mekgineer_steamriggerAI (_Creature);
}

#define SPELL_DISPEL_MAGIC          17201
#define SPELL_REPAIR                31532
#define H_SPELL_REPAIR              37936

#define MAX_REPAIR_RANGE            (13.0f)                 //we should be at least at this range for repair
#define MIN_REPAIR_RANGE            (7.0f)                  //we can stop movement at this range to repair but not required

struct MANGOS_DLL_DECL mob_steamrigger_mechanicAI : public ScriptedAI
{
    mob_steamrigger_mechanicAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;

    uint32 Repair_Timer;
    bool HeroicMode;

    void Reset()
    {
        HeroicMode = m_creature->GetMap()->IsHeroic();
        Repair_Timer = 2000;
    }

    void MoveInLineOfSight(Unit* who)
    {
        //react only if attacked
        return;
    }

    void Aggro(Unit *who) { }

    void UpdateAI(const uint32 diff) 
    {
        if( Repair_Timer < diff )
        {
            if( pInstance && pInstance->GetData64(DATA_MEKGINEERSTEAMRIGGER) && pInstance->GetData(TYPE_MEKGINEER_STEAMRIGGER) == IN_PROGRESS)
            {
                if( Unit* pMekgineer = Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_MEKGINEERSTEAMRIGGER)) )
                {
                    if( m_creature->IsWithinDistInMap(pMekgineer, MAX_REPAIR_RANGE) )
                    {
                        //are we already channeling? Doesn't work very well, find better check?
                        if( !m_creature->GetUInt32Value(UNIT_CHANNEL_SPELL) )
                        {
                            //m_creature->GetMotionMaster()->MovementExpired();
                            //m_creature->GetMotionMaster()->MoveIdle();

                            if( HeroicMode ) DoCast(m_creature,H_SPELL_REPAIR,true);
                            else DoCast(m_creature,SPELL_REPAIR,true);
                        }
                        Repair_Timer = 5000;
                    }
                    else
                    {
                        //m_creature->GetMotionMaster()->MovementExpired();
                        //m_creature->GetMotionMaster()->MoveFollow(pMekgineer,0,0);
                    }
                }
            }else Repair_Timer = 5000;
        }else Repair_Timer -= diff;

        if( !m_creature->SelectHostilTarget() || !m_creature->getVictim() )
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_steamrigger_mechanic(Creature *_Creature)
{
    return new mob_steamrigger_mechanicAI (_Creature);
}

void AddSC_boss_mekgineer_steamrigger()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_mekgineer_steamrigger";
    newscript->GetAI = GetAI_boss_mekgineer_steamrigger;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mob_steamrigger_mechanic";
    newscript->GetAI = GetAI_mob_steamrigger_mechanic;
    m_scripts[nrscripts++] = newscript;
}