using System;
using System.Collections.Generic;

namespace MapTool.NPC
{
    /// <summary>
    /// NPC entry for export (matches Npc_Load.txt format)
    /// Format: NpcID | MapID | PosX | PosY | ScriptFile | Name | Level | IsLoad
    /// </summary>
    public class NpcEntry
    {
        public int NpcID { get; set; }
        public int MapID { get; set; }
        public int PosX { get; set; }
        public int PosY { get; set; }
        public string ScriptFile { get; set; }
        public string Name { get; set; }
        public int Level { get; set; }
        public int IsLoad { get; set; }

        public override string ToString()
        {
            return $"{NpcID}\t{MapID}\t{PosX}\t{PosY}\t{ScriptFile}\t{Name}\t{Level}\t{IsLoad}";
        }
    }

    /// <summary>
    /// Object entry (matches Obj format)
    /// Format: ObjID | MapID | PosX | PosY | Dir | State | ScriptFile | IsLoad
    /// </summary>
    public class ObjectEntry
    {
        public int ObjID { get; set; }
        public int MapID { get; set; }
        public int PosX { get; set; }
        public int PosY { get; set; }
        public int Dir { get; set; }
        public int State { get; set; }
        public string ScriptFile { get; set; }
        public int IsLoad { get; set; }

        public override string ToString()
        {
            return $"{ObjID}\t{MapID}\t{PosX}\t{PosY}\t{Dir}\t{State}\t{ScriptFile}\t{IsLoad}";
        }

        /// <summary>
        /// Convert to NpcEntry for display on map
        /// </summary>
        public NpcEntry ToNpcEntry()
        {
            return new NpcEntry
            {
                NpcID = ObjID,
                MapID = MapID,
                PosX = PosX,
                PosY = PosY,
                ScriptFile = ScriptFile,
                Name = $"Obj_{ObjID}",
                Level = 1,
                IsLoad = IsLoad
            };
        }
    }

    /// <summary>
    /// NPC resource information from NpcResKind.txt
    /// </summary>
    public class NpcResKind
    {
        public string CharacterName { get; set; }  // e.g., "enemy003"
        public string CharacterType { get; set; }  // "NormalNpc" or "SpecialNpc"
        public string ResFilePath { get; set; }    // e.g., "spr\\npcres\\enemy\\enemy003"
    }

    /// <summary>
    /// NPC sprite resource mapping from NpcNormalRes.txt
    /// Maps NPC name to SPR files for each action
    /// </summary>
    public class NpcSpriteRes
    {
        public string NpcName { get; set; }

        // Action sprite files
        public string FightStand { get; set; }      // Fighting stance
        public string NormalStand1 { get; set; }    // Normal standing
        public string NormalWalk { get; set; }      // Walking
        public string Attack1 { get; set; }         // Attack 1
        public string Attack2 { get; set; }         // Attack 2
        public string Attack3 { get; set; }         // Attack 3
        public string CastSkill { get; set; }       // Casting skill
        public string Hurt { get; set; }            // Taking damage
        public string Die { get; set; }             // Death animation
        public string Run { get; set; }             // Running

        /// <summary>
        /// Get SPR file for action
        /// </summary>
        public string GetActionSprite(NpcAction action)
        {
            switch (action)
            {
                case NpcAction.FightStand: return FightStand;
                case NpcAction.NormalStand: return NormalStand1;
                case NpcAction.Walk: return NormalWalk;
                case NpcAction.Attack1: return Attack1;
                case NpcAction.Attack2: return Attack2;
                case NpcAction.Attack3: return Attack3;
                case NpcAction.CastSkill: return CastSkill;
                case NpcAction.Hurt: return Hurt;
                case NpcAction.Die: return Die;
                case NpcAction.Run: return Run;
                default: return FightStand ?? NormalStand1;
            }
        }
    }

    /// <summary>
    /// NPC sprite information from NpcNormalSprInfo.txt
    /// Contains frame dimensions and timing
    /// Format: Width, Directions, Interval
    /// </summary>
    public class NpcSpriteInfo
    {
        public string NpcName { get; set; }

        public SpriteFrameInfo FightStand { get; set; }
        public SpriteFrameInfo NormalStand1 { get; set; }
        public SpriteFrameInfo NormalWalk { get; set; }
        public SpriteFrameInfo Attack1 { get; set; }
        public SpriteFrameInfo Attack2 { get; set; }
        public SpriteFrameInfo Attack3 { get; set; }
        public SpriteFrameInfo CastSkill { get; set; }
        public SpriteFrameInfo Hurt { get; set; }
        public SpriteFrameInfo Die { get; set; }
        public SpriteFrameInfo Run { get; set; }
    }

    /// <summary>
    /// Frame information: Width, Directions, Interval
    /// </summary>
    public class SpriteFrameInfo
    {
        public int Width { get; set; }
        public int Directions { get; set; }
        public int Interval { get; set; }  // Milliseconds

        public static SpriteFrameInfo Parse(string value)
        {
            // Format: "48,8,200" (width, directions, interval)
            string[] parts = value.Split(',');
            if (parts.Length >= 3)
            {
                return new SpriteFrameInfo
                {
                    Width = int.Parse(parts[0].Trim()),
                    Directions = int.Parse(parts[1].Trim()),
                    Interval = int.Parse(parts[2].Trim())
                };
            }
            return null;
        }
    }

    /// <summary>
    /// NPC actions/animations
    /// </summary>
    public enum NpcAction
    {
        FightStand,
        NormalStand,
        Walk,
        Attack1,
        Attack2,
        Attack3,
        CastSkill,
        Hurt,
        Die,
        Run
    }

    /// <summary>
    /// Complete NPC resource data
    /// </summary>
    public class NpcResource
    {
        public int NpcID { get; set; }
        public string NpcName { get; set; }
        public NpcResKind ResKind { get; set; }
        public NpcSpriteRes SpriteRes { get; set; }
        public NpcSpriteInfo SpriteInfo { get; set; }

        /// <summary>
        /// Get full SPR file path
        /// </summary>
        public string GetSprFilePath(NpcAction action)
        {
            if (ResKind == null || SpriteRes == null)
                return null;

            string sprFile = SpriteRes.GetActionSprite(action);
            if (string.IsNullOrEmpty(sprFile))
                return null;

            // Combine: ResFilePath + SprFile
            // e.g., "spr\\npcres\\enemy\\enemy003" + "\\enemy003_st.spr"
            return $"{ResKind.ResFilePath}\\{sprFile}";
        }
    }
}
