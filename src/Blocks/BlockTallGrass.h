
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"





class cBlockTallGrassHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;
public:
	cBlockTallGrassHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Drop seeds, sometimes
		cFastRandom Random;
		if (Random.NextInt(8) == 0)
		{
			a_Pickups.push_back(cItem(E_ITEM_SEEDS, 1, 0));
		}
	}

	virtual void DropBlock(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_CanDrop) override
	{
		if (a_CanDrop && (a_Digger != nullptr) && (a_Digger->GetEquippedWeapon().m_ItemType == E_ITEM_SHEARS))
		{
			NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
			cItems Drops;
			Drops.Add(m_BlockType, 1, Meta);

			// Allow plugins to modify the pickups:
			a_BlockPluginInterface.CallHookBlockToPickups(a_Digger, a_BlockX, a_BlockY, a_BlockZ, m_BlockType, Meta, Drops);

			// Spawn the pickups:
			if (!Drops.empty())
			{
				MTRand r1;

				// Mid-block position first
				double MicroX, MicroY, MicroZ;
				MicroX = a_BlockX + 0.5;
				MicroY = a_BlockY + 0.5;
				MicroZ = a_BlockZ + 0.5;

				// Add random offset second
				MicroX += r1.rand(1) - 0.5;
				MicroZ += r1.rand(1) - 0.5;

				a_WorldInterface.SpawnItemPickups(Drops, MicroX, MicroY, MicroZ);
			}
			return;
		}

		super::DropBlock(a_ChunkInterface, a_WorldInterface, a_BlockPluginInterface, a_Digger, a_BlockX, a_BlockY, a_BlockZ, a_CanDrop);
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}

		BLOCKTYPE BelowBlock = a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ);
		return IsBlockTypeOfDirt(BelowBlock);
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




