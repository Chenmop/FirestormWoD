/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "throne_of_thunder.h"

enum eSpells
{
    SPELL_HARD_STARE        = 133765,
    SPELL_FORCE_OF_WILL     = 136413
};

enum eEvents
{
    EVENT_HARD_STARE        = 1,
    EVENT_FORCE_OF_WILL     = 2
};

// Durumu the forgotten - 68036
class boss_durumu : public CreatureScript
{
    public:
        boss_durumu() : CreatureScript("boss_durumu") { }

        struct boss_durumuAI : public BossAI
        {
            boss_durumuAI(Creature* p_Creature) : BossAI(p_Creature, DATA_DURUMU_THE_FORGOTTEN)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset()
            {
                m_Events.Reset();

                _Reset();

                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();

                summons.DespawnAll();

                if (m_Instance)
                {
                    if (m_Instance->GetBossState(FAIL))
                        m_Instance->SetBossState(DATA_DURUMU_THE_FORGOTTEN, NOT_STARTED);
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.Reset();
                m_Events.ScheduleEvent(EVENT_HARD_STARE, 10000);
                m_Events.ScheduleEvent(EVENT_FORCE_OF_WILL, 20000);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
            }

            void DoAction(const int32 /*p_Action*/)
            {
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_HARD_STARE:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_HARD_STARE, false);
                        m_Events.ScheduleEvent(EVENT_HARD_STARE, 12000);
                        break;
                    case EVENT_FORCE_OF_WILL:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_FORCE_OF_WILL, false);
                        m_Events.ScheduleEvent(EVENT_FORCE_OF_WILL, 6000);
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_durumuAI(p_Creature);
        }
};

// Arterial Cut (aura) - 133768
class spell_arterial_cut : public SpellScriptLoader
{
public:
    spell_arterial_cut() : SpellScriptLoader("spell_arterial_cut") { }

    class spell_arterial_cut_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_arterial_cut_AuraScript);

        void OnTick(constAuraEffectPtr /*aurEff*/)
        {
            Unit* l_Caster = GetCaster();

            if (!l_Caster)
                return;

            if (Player* l_Player = l_Caster->ToPlayer())
                if (l_Player->GetHealth() == l_Player->GetMaxHealth())
                    this->Remove();
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_arterial_cut_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_arterial_cut_AuraScript();
    }
};

void AddSC_boss_durumu()
{
    new boss_durumu();
    new spell_arterial_cut();
}
