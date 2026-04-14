#ifndef __Thai2_DATA_UI_H_
#define __Thai2_DATA_UI_H_

#define MAX_Thai2_DATA_UI              1

struct Thai2_DATA_UI
{
       struct Thai2_DATA_ID
       {
            wchar_t*        WData;
            wchar_t*        OData;
       };
       
       Thai2_DATA_ID
       DataUI[MAX_Thai2_DATA_UI];
       
       ~Thai2_DATA_UI()
       {
            for ( int i =0; i <MAX_Thai2_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       Thai2_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[79 +1];
            DataUI[ 0].WData[ 0] =0xe17;
            DataUI[ 0].WData[ 1] =0xe35;
            DataUI[ 0].WData[ 2] =0xe48;
            DataUI[ 0].WData[ 3] =0x20;
            DataUI[ 0].WData[ 4] =0xe14;
            DataUI[ 0].WData[ 5] =0xe35;
            DataUI[ 0].WData[ 6] =0xe46;
            DataUI[ 0].WData[ 7] =0x20;
            DataUI[ 0].WData[ 8] =0xe15;
            DataUI[ 0].WData[ 9] =0xe39;
            DataUI[ 0].WData[10] =0xe49;
            DataUI[ 0].WData[11] =0x20;
            DataUI[ 0].WData[12] =0xe15;
            DataUI[ 0].WData[13] =0xe38;
            DataUI[ 0].WData[14] =0xe4a;
            DataUI[ 0].WData[15] =0x20;
            DataUI[ 0].WData[16] =0xe15;
            DataUI[ 0].WData[17] =0xe35;
            DataUI[ 0].WData[18] =0xe48;
            DataUI[ 0].WData[19] =0x20;
            DataUI[ 0].WData[20] =0xe15;
            DataUI[ 0].WData[21] =0xe39;
            DataUI[ 0].WData[22] =0xe48;
            DataUI[ 0].WData[23] =0x20;
            DataUI[ 0].WData[24] =0xe14;
            DataUI[ 0].WData[25] =0xe34;
            DataUI[ 0].WData[26] =0xe49;
            DataUI[ 0].WData[27] =0x20;
            DataUI[ 0].WData[28] =0xe15;
            DataUI[ 0].WData[29] =0xe35;
            DataUI[ 0].WData[30] =0xe4b;
            DataUI[ 0].WData[31] =0x20;
            DataUI[ 0].WData[32] =0xe15;
            DataUI[ 0].WData[33] =0xe35;
            DataUI[ 0].WData[34] =0xe49;
            DataUI[ 0].WData[35] =0x20;
            DataUI[ 0].WData[36] =0xe14;
            DataUI[ 0].WData[37] =0xe34;
            DataUI[ 0].WData[38] =0xe48;
            DataUI[ 0].WData[39] =0x20;
            DataUI[ 0].WData[40] =0xe16;
            DataUI[ 0].WData[41] =0xe35;
            DataUI[ 0].WData[42] =0xe48;
            DataUI[ 0].WData[43] =0x20;
            DataUI[ 0].WData[44] =0xe14;
            DataUI[ 0].WData[45] =0xe35;
            DataUI[ 0].WData[46] =0xe49;
            DataUI[ 0].WData[47] =0x20;
            DataUI[ 0].WData[48] =0xe17;
            DataUI[ 0].WData[49] =0xe39;
            DataUI[ 0].WData[50] =0xe49;
            DataUI[ 0].WData[51] =0x20;
            DataUI[ 0].WData[52] =0xe14;
            DataUI[ 0].WData[53] =0xe49;
            DataUI[ 0].WData[54] =0xe46;
            DataUI[ 0].WData[55] =0x20;
            DataUI[ 0].WData[56] =0xe15;
            DataUI[ 0].WData[57] =0xe37;
            DataUI[ 0].WData[58] =0xe48;
            DataUI[ 0].WData[59] =0x20;
            DataUI[ 0].WData[60] =0xe15;
            DataUI[ 0].WData[61] =0xe31;
            DataUI[ 0].WData[62] =0xe49;
            DataUI[ 0].WData[63] =0x20;
            DataUI[ 0].WData[64] =0xe17;
            DataUI[ 0].WData[65] =0xe34;
            DataUI[ 0].WData[66] =0xe49;
            DataUI[ 0].WData[67] =0x20;
            DataUI[ 0].WData[68] =0xe15;
            DataUI[ 0].WData[69] =0xe34;
            DataUI[ 0].WData[70] =0xe49;
            DataUI[ 0].WData[71] =0x20;
            DataUI[ 0].WData[72] =0xe17;
            DataUI[ 0].WData[73] =0xe4c;
            DataUI[ 0].WData[74] =0xe46;
            DataUI[ 0].WData[75] =0x20;
            DataUI[ 0].WData[76] =0xe14;
            DataUI[ 0].WData[77] =0xe34;
            DataUI[ 0].WData[78] =0xe4a;
            DataUI[ 0].WData[79] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
