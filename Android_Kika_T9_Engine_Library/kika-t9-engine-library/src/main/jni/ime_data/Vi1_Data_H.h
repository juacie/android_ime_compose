#ifndef __Vi1_DATA_UI_H_
#define __Vi1_DATA_UI_H_

#define MAX_Vi1_DATA_UI                20

struct Vi1_DATA_UI
{
       struct Vi1_DATA_ID
       {
            wchar_t*        WData;
            wchar_t*        OData;
       };
       
       Vi1_DATA_ID
       DataUI[MAX_Vi1_DATA_UI];
       
       ~Vi1_DATA_UI()
       {
            for ( int i =0; i <MAX_Vi1_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       Vi1_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[4 +1];
            DataUI[ 0].WData[ 0] =0x4e;
            DataUI[ 0].WData[ 1] =0x68;
            DataUI[ 0].WData[ 2] =0xe2;
            DataUI[ 0].WData[ 3] =0x6e;
            DataUI[ 0].WData[ 4] =0;

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[4 +1];
            DataUI[ 1].WData[ 0] =0x6e;
            DataUI[ 1].WData[ 1] =0x68;
            DataUI[ 1].WData[ 2] =0x1ead;
            DataUI[ 1].WData[ 3] =0x6e;
            DataUI[ 1].WData[ 4] =0;

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[4 +1];
            DataUI[ 2].WData[ 0] =0x6e;
            DataUI[ 2].WData[ 1] =0x68;
            DataUI[ 2].WData[ 2] =0x61;
            DataUI[ 2].WData[ 3] =0x6e;
            DataUI[ 2].WData[ 4] =0;

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[4 +1];
            DataUI[ 3].WData[ 0] =0x6e;
            DataUI[ 3].WData[ 1] =0x67;
            DataUI[ 3].WData[ 2] =0xe2;
            DataUI[ 3].WData[ 3] =0x6e;
            DataUI[ 3].WData[ 4] =0;

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[4 +1];
            DataUI[ 4].WData[ 0] =0x6e;
            DataUI[ 4].WData[ 1] =0x68;
            DataUI[ 4].WData[ 2] =0x1eaf;
            DataUI[ 4].WData[ 3] =0x6e;
            DataUI[ 4].WData[ 4] =0;

            //------------------------------------------------------
            // Index 5
            //------------------------------------------------------
            DataUI[ 5].WData =new wchar_t[4 +1];
            DataUI[ 5].WData[ 0] =0x6e;
            DataUI[ 5].WData[ 1] =0x67;
            DataUI[ 5].WData[ 2] =0x61;
            DataUI[ 5].WData[ 3] =0x6e;
            DataUI[ 5].WData[ 4] =0;

            //------------------------------------------------------
            // Index 6
            //------------------------------------------------------
            DataUI[ 6].WData =new wchar_t[4 +1];
            DataUI[ 6].WData[ 0] =0x6e;
            DataUI[ 6].WData[ 1] =0x68;
            DataUI[ 6].WData[ 2] =0x1ea7;
            DataUI[ 6].WData[ 3] =0x6d;
            DataUI[ 6].WData[ 4] =0;

            //------------------------------------------------------
            // Index 7
            //------------------------------------------------------
            DataUI[ 7].WData =new wchar_t[4 +1];
            DataUI[ 7].WData[ 0] =0x6e;
            DataUI[ 7].WData[ 1] =0x67;
            DataUI[ 7].WData[ 2] =0x1eaf;
            DataUI[ 7].WData[ 3] =0x6e;
            DataUI[ 7].WData[ 4] =0;

            //------------------------------------------------------
            // Index 8
            //------------------------------------------------------
            DataUI[ 8].WData =new wchar_t[4 +1];
            DataUI[ 8].WData[ 0] =0x4e;
            DataUI[ 8].WData[ 1] =0x67;
            DataUI[ 8].WData[ 2] =0xe0;
            DataUI[ 8].WData[ 3] =0x6e;
            DataUI[ 8].WData[ 4] =0;

            //------------------------------------------------------
            // Index 9
            //------------------------------------------------------
            DataUI[ 9].WData =new wchar_t[4 +1];
            DataUI[ 9].WData[ 0] =0x6e;
            DataUI[ 9].WData[ 1] =0x68;
            DataUI[ 9].WData[ 2] =0x61;
            DataUI[ 9].WData[ 3] =0x6d;
            DataUI[ 9].WData[ 4] =0;

            //------------------------------------------------------
            // Index 10
            //------------------------------------------------------
            DataUI[10].WData =new wchar_t[4 +1];
            DataUI[10].WData[ 0] =0x6e;
            DataUI[10].WData[ 1] =0x67;
            DataUI[10].WData[ 2] =0x1eaf;
            DataUI[10].WData[ 3] =0x6d;
            DataUI[10].WData[ 4] =0;

            //------------------------------------------------------
            // Index 11
            //------------------------------------------------------
            DataUI[11].WData =new wchar_t[4 +1];
            DataUI[11].WData[ 0] =0x6e;
            DataUI[11].WData[ 1] =0x68;
            DataUI[11].WData[ 2] =0x1eaf;
            DataUI[11].WData[ 3] =0x6d;
            DataUI[11].WData[ 4] =0;

            //------------------------------------------------------
            // Index 12
            //------------------------------------------------------
            DataUI[12].WData =new wchar_t[4 +1];
            DataUI[12].WData[ 0] =0x6e;
            DataUI[12].WData[ 1] =0x67;
            DataUI[12].WData[ 2] =0x103;
            DataUI[12].WData[ 3] =0x6e;
            DataUI[12].WData[ 4] =0;

            //------------------------------------------------------
            // Index 13
            //------------------------------------------------------
            DataUI[13].WData =new wchar_t[4 +1];
            DataUI[13].WData[ 0] =0x6e;
            DataUI[13].WData[ 1] =0x68;
            DataUI[13].WData[ 2] =0x1eb1;
            DataUI[13].WData[ 3] =0x6d;
            DataUI[13].WData[ 4] =0;

            //------------------------------------------------------
            // Index 14
            //------------------------------------------------------
            DataUI[14].WData =new wchar_t[4 +1];
            DataUI[14].WData[ 0] =0x6e;
            DataUI[14].WData[ 1] =0x67;
            DataUI[14].WData[ 2] =0xe2;
            DataUI[14].WData[ 3] =0x6d;
            DataUI[14].WData[ 4] =0;

            //------------------------------------------------------
            // Index 15
            //------------------------------------------------------
            DataUI[15].WData =new wchar_t[4 +1];
            DataUI[15].WData[ 0] =0x6e;
            DataUI[15].WData[ 1] =0x67;
            DataUI[15].WData[ 2] =0xe1;
            DataUI[15].WData[ 3] =0x6f;
            DataUI[15].WData[ 4] =0;

            //------------------------------------------------------
            // Index 16
            //------------------------------------------------------
            DataUI[16].WData =new wchar_t[4 +1];
            DataUI[16].WData[ 0] =0x6e;
            DataUI[16].WData[ 1] =0x68;
            DataUI[16].WData[ 2] =0x1ea3;
            DataUI[16].WData[ 3] =0x6d;
            DataUI[16].WData[ 4] =0;

            //------------------------------------------------------
            // Index 17
            //------------------------------------------------------
            DataUI[17].WData =new wchar_t[4 +1];
            DataUI[17].WData[ 0] =0x6e;
            DataUI[17].WData[ 1] =0x68;
            DataUI[17].WData[ 2] =0x1ea5;
            DataUI[17].WData[ 3] =0x6e;
            DataUI[17].WData[ 4] =0;

            //------------------------------------------------------
            // Index 18
            //------------------------------------------------------
            DataUI[18].WData =new wchar_t[4 +1];
            DataUI[18].WData[ 0] =0x6e;
            DataUI[18].WData[ 1] =0x68;
            DataUI[18].WData[ 2] =0x1eab;
            DataUI[18].WData[ 3] =0x6e;
            DataUI[18].WData[ 4] =0;

            //------------------------------------------------------
            // Index 19
            //------------------------------------------------------
            DataUI[19].WData =new wchar_t[4 +1];
            DataUI[19].WData[ 0] =0x6e;
            DataUI[19].WData[ 1] =0x67;
            DataUI[19].WData[ 2] =0xe1;
            DataUI[19].WData[ 3] =0x6e;
            DataUI[19].WData[ 4] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
