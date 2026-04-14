#ifndef __Vi3_DATA_UI_H_
#define __Vi3_DATA_UI_H_

#define MAX_Vi3_DATA_UI                18

struct Vi3_DATA_UI
{
       struct Vi3_DATA_ID
       {
            wchar_t*        WData;
            wchar_t*        OData;
       };
       
       Vi3_DATA_ID
       DataUI[MAX_Vi3_DATA_UI];
       
       ~Vi3_DATA_UI()
       {
            for ( int i =0; i <MAX_Vi3_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       Vi3_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[3 +1];
            DataUI[ 0].WData[ 0] =0x73;
            DataUI[ 0].WData[ 1] =0x61;
            DataUI[ 0].WData[ 2] =0x75;
            DataUI[ 0].WData[ 3] =0;

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[3 +1];
            DataUI[ 1].WData[ 0] =0x72;
            DataUI[ 1].WData[ 1] =0x1ea5;
            DataUI[ 1].WData[ 2] =0x74;
            DataUI[ 1].WData[ 3] =0;

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[3 +1];
            DataUI[ 2].WData[ 0] =0x73;
            DataUI[ 2].WData[ 1] =0xe1;
            DataUI[ 2].WData[ 2] =0x74;
            DataUI[ 2].WData[ 3] =0;

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[3 +1];
            DataUI[ 3].WData[ 0] =0x73;
            DataUI[ 3].WData[ 1] =0xe2;
            DataUI[ 3].WData[ 2] =0x75;
            DataUI[ 3].WData[ 3] =0;

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[3 +1];
            DataUI[ 4].WData[ 0] =0x72;
            DataUI[ 4].WData[ 1] =0x61;
            DataUI[ 4].WData[ 2] =0x75;
            DataUI[ 4].WData[ 3] =0;

            //------------------------------------------------------
            // Index 5
            //------------------------------------------------------
            DataUI[ 5].WData =new wchar_t[3 +1];
            DataUI[ 5].WData[ 0] =0x73;
            DataUI[ 5].WData[ 1] =0x1eaf;
            DataUI[ 5].WData[ 2] =0x74;
            DataUI[ 5].WData[ 3] =0;

            //------------------------------------------------------
            // Index 6
            //------------------------------------------------------
            DataUI[ 6].WData =new wchar_t[3 +1];
            DataUI[ 6].WData[ 0] =0x72;
            DataUI[ 6].WData[ 1] =0x1ea7;
            DataUI[ 6].WData[ 2] =0x75;
            DataUI[ 6].WData[ 3] =0;

            //------------------------------------------------------
            // Index 7
            //------------------------------------------------------
            DataUI[ 7].WData =new wchar_t[3 +1];
            DataUI[ 7].WData[ 0] =0x73;
            DataUI[ 7].WData[ 1] =0xe1;
            DataUI[ 7].WData[ 2] =0x75;
            DataUI[ 7].WData[ 3] =0;

            //------------------------------------------------------
            // Index 8
            //------------------------------------------------------
            DataUI[ 8].WData =new wchar_t[3 +1];
            DataUI[ 8].WData[ 0] =0x72;
            DataUI[ 8].WData[ 1] =0xe2;
            DataUI[ 8].WData[ 2] =0x75;
            DataUI[ 8].WData[ 3] =0;

            //------------------------------------------------------
            // Index 9
            //------------------------------------------------------
            DataUI[ 9].WData =new wchar_t[3 +1];
            DataUI[ 9].WData[ 0] =0x73;
            DataUI[ 9].WData[ 1] =0x1ea7;
            DataUI[ 9].WData[ 2] =0x75;
            DataUI[ 9].WData[ 3] =0;

            //------------------------------------------------------
            // Index 10
            //------------------------------------------------------
            DataUI[10].WData =new wchar_t[3 +1];
            DataUI[10].WData[ 0] =0x72;
            DataUI[10].WData[ 1] =0xe1;
            DataUI[10].WData[ 2] =0x74;
            DataUI[10].WData[ 3] =0;

            //------------------------------------------------------
            // Index 11
            //------------------------------------------------------
            DataUI[11].WData =new wchar_t[3 +1];
            DataUI[11].WData[ 0] =0x73;
            DataUI[11].WData[ 1] =0x1ea5;
            DataUI[11].WData[ 2] =0x75;
            DataUI[11].WData[ 3] =0;

            //------------------------------------------------------
            // Index 12
            //------------------------------------------------------
            DataUI[12].WData =new wchar_t[3 +1];
            DataUI[12].WData[ 0] =0x73;
            DataUI[12].WData[ 1] =0x1ea1;
            DataUI[12].WData[ 2] =0x74;
            DataUI[12].WData[ 3] =0;

            //------------------------------------------------------
            // Index 13
            //------------------------------------------------------
            DataUI[13].WData =new wchar_t[3 +1];
            DataUI[13].WData[ 0] =0x72;
            DataUI[13].WData[ 1] =0x1eaf;
            DataUI[13].WData[ 2] =0x74;
            DataUI[13].WData[ 3] =0;

            //------------------------------------------------------
            // Index 14
            //------------------------------------------------------
            DataUI[14].WData =new wchar_t[3 +1];
            DataUI[14].WData[ 0] =0x73;
            DataUI[14].WData[ 1] =0x1ea5;
            DataUI[14].WData[ 2] =0x74;
            DataUI[14].WData[ 3] =0;

            //------------------------------------------------------
            // Index 15
            //------------------------------------------------------
            DataUI[15].WData =new wchar_t[3 +1];
            DataUI[15].WData[ 0] =0x73;
            DataUI[15].WData[ 1] =0x1ead;
            DataUI[15].WData[ 2] =0x74;
            DataUI[15].WData[ 3] =0;

            //------------------------------------------------------
            // Index 16
            //------------------------------------------------------
            DataUI[16].WData =new wchar_t[3 +1];
            DataUI[16].WData[ 0] =0x50;
            DataUI[16].WData[ 1] =0xe2;
            DataUI[16].WData[ 2] =0x75;
            DataUI[16].WData[ 3] =0;

            //------------------------------------------------------
            // Index 17
            //------------------------------------------------------
            DataUI[17].WData =new wchar_t[4 +1];
            DataUI[17].WData[ 0] =0x70;
            DataUI[17].WData[ 1] =0x61;
            DataUI[17].WData[ 2] =0x74;
            DataUI[17].WData[ 3] =0xea;
            DataUI[17].WData[ 4] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
