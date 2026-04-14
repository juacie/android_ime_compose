#ifndef __Thai3_DATA_UI_H_
#define __Thai3_DATA_UI_H_

#define MAX_Thai3_DATA_UI                    20

struct Thai3_DATA_UI
{
       struct Thai3_DATA_ID
       {
            wchar_t*        WData;
            wchar_t*        OData;
       };
       
       Thai3_DATA_ID
       DataUI[MAX_Thai3_DATA_UI];
       
       ~Thai3_DATA_UI()
       {
            for ( int i =0; i <MAX_Thai3_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       Thai3_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[5 +1];
            DataUI[ 0].WData[ 0] =0xe01;
            DataUI[ 0].WData[ 1] =0xe25;
            DataUI[ 0].WData[ 2] =0xe34;
            DataUI[ 0].WData[ 3] =0xe48;
            DataUI[ 0].WData[ 4] =0xe19;
            DataUI[ 0].WData[ 5] =0;

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[5 +1];
            DataUI[ 1].WData[ 0] =0xe01;
            DataUI[ 1].WData[ 1] =0xe23;
            DataUI[ 1].WData[ 2] =0xe38;
            DataUI[ 1].WData[ 3] =0xe4a;
            DataUI[ 1].WData[ 4] =0xe1b;
            DataUI[ 1].WData[ 5] =0;

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[5 +1];
            DataUI[ 2].WData[ 0] =0xe04;
            DataUI[ 2].WData[ 1] =0xe25;
            DataUI[ 2].WData[ 2] =0xe37;
            DataUI[ 2].WData[ 3] =0xe48;
            DataUI[ 2].WData[ 4] =0xe19;
            DataUI[ 2].WData[ 5] =0;

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[5 +1];
            DataUI[ 3].WData[ 0] =0xe01;
            DataUI[ 3].WData[ 1] =0xe25;
            DataUI[ 3].WData[ 2] =0xe31;
            DataUI[ 3].WData[ 3] =0xe49;
            DataUI[ 3].WData[ 4] =0xe19;
            DataUI[ 3].WData[ 5] =0;

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[5 +1];
            DataUI[ 4].WData[ 0] =0xe01;
            DataUI[ 4].WData[ 1] =0xe25;
            DataUI[ 4].WData[ 2] =0xe31;
            DataUI[ 4].WData[ 3] =0xe48;
            DataUI[ 4].WData[ 4] =0xe19;
            DataUI[ 4].WData[ 5] =0;

            //------------------------------------------------------
            // Index 5
            //------------------------------------------------------
            DataUI[ 5].WData =new wchar_t[5 +1];
            DataUI[ 5].WData[ 0] =0xe04;
            DataUI[ 5].WData[ 1] =0xe23;
            DataUI[ 5].WData[ 2] =0xe31;
            DataUI[ 5].WData[ 3] =0xe49;
            DataUI[ 5].WData[ 4] =0xe1a;
            DataUI[ 5].WData[ 5] =0;

            //------------------------------------------------------
            // Index 6
            //------------------------------------------------------
            DataUI[ 6].WData =new wchar_t[5 +1];
            DataUI[ 6].WData[ 0] =0xe04;
            DataUI[ 6].WData[ 1] =0xe27;
            DataUI[ 6].WData[ 2] =0xe31;
            DataUI[ 6].WData[ 3] =0xe48;
            DataUI[ 6].WData[ 4] =0xe19;
            DataUI[ 6].WData[ 5] =0;

            //------------------------------------------------------
            // Index 7
            //------------------------------------------------------
            DataUI[ 7].WData =new wchar_t[5 +1];
            DataUI[ 7].WData[ 0] =0xe04;
            DataUI[ 7].WData[ 1] =0xe23;
            DataUI[ 7].WData[ 2] =0xe31;
            DataUI[ 7].WData[ 3] =0xe49;
            DataUI[ 7].WData[ 4] =0xe19;
            DataUI[ 7].WData[ 5] =0;

            //------------------------------------------------------
            // Index 8
            //------------------------------------------------------
            DataUI[ 8].WData =new wchar_t[5 +1];
            DataUI[ 8].WData[ 0] =0xe01;
            DataUI[ 8].WData[ 1] =0xe25;
            DataUI[ 8].WData[ 2] =0xe31;
            DataUI[ 8].WData[ 3] =0xe31;
            DataUI[ 8].WData[ 4] =0xe1a;
            DataUI[ 8].WData[ 5] =0;

            //------------------------------------------------------
            // Index 9
            //------------------------------------------------------
            DataUI[ 9].WData =new wchar_t[5 +1];
            DataUI[ 9].WData[ 0] =0xe02;
            DataUI[ 9].WData[ 1] =0xe23;
            DataUI[ 9].WData[ 2] =0xe31;
            DataUI[ 9].WData[ 3] =0xe48;
            DataUI[ 9].WData[ 4] =0xe1a;
            DataUI[ 9].WData[ 5] =0;

            //------------------------------------------------------
            // Index 10
            //------------------------------------------------------
            DataUI[10].WData =new wchar_t[5 +1];
            DataUI[10].WData[ 0] =0xe01;
            DataUI[10].WData[ 1] =0xe23;
            DataUI[10].WData[ 2] =0xe38;
            DataUI[10].WData[ 3] =0xe48;
            DataUI[10].WData[ 4] =0xe19;
            DataUI[10].WData[ 5] =0;

            //------------------------------------------------------
            // Index 11
            //------------------------------------------------------
            DataUI[11].WData =new wchar_t[5 +1];
            DataUI[11].WData[ 0] =0xe04;
            DataUI[11].WData[ 1] =0xe23;
            DataUI[11].WData[ 2] =0xe37;
            DataUI[11].WData[ 3] =0xe49;
            DataUI[11].WData[ 4] =0xe19;
            DataUI[11].WData[ 5] =0;

            //------------------------------------------------------
            // Index 12
            //------------------------------------------------------
            DataUI[12].WData =new wchar_t[5 +1];
            DataUI[12].WData[ 0] =0xe04;
            DataUI[12].WData[ 1] =0xe23;
            DataUI[12].WData[ 2] =0xe38;
            DataUI[12].WData[ 3] =0xe48;
            DataUI[12].WData[ 4] =0xe19;
            DataUI[12].WData[ 5] =0;

            //------------------------------------------------------
            // Index 13
            //------------------------------------------------------
            DataUI[13].WData =new wchar_t[5 +1];
            DataUI[13].WData[ 0] =0xe04;
            DataUI[13].WData[ 1] =0xe23;
            DataUI[13].WData[ 2] =0xe31;
            DataUI[13].WData[ 3] =0xe48;
            DataUI[13].WData[ 4] =0xe19;
            DataUI[13].WData[ 5] =0;

            //------------------------------------------------------
            // Index 14
            //------------------------------------------------------
            DataUI[14].WData =new wchar_t[5 +1];
            DataUI[14].WData[ 0] =0xe02;
            DataUI[14].WData[ 1] =0xe27;
            DataUI[14].WData[ 2] =0xe31;
            DataUI[14].WData[ 3] =0xe49;
            DataUI[14].WData[ 4] =0xe19;
            DataUI[14].WData[ 5] =0;

            //------------------------------------------------------
            // Index 15
            //------------------------------------------------------
            DataUI[15].WData =new wchar_t[6 +1];
            DataUI[15].WData[ 0] =0xe04;
            DataUI[15].WData[ 1] =0xe23;
            DataUI[15].WData[ 2] =0xe31;
            DataUI[15].WData[ 3] =0xe49;
            DataUI[15].WData[ 4] =0xe1a;
            DataUI[15].WData[ 5] =0xe1a;
            DataUI[15].WData[ 6] =0;

            //------------------------------------------------------
            // Index 16
            //------------------------------------------------------
            DataUI[16].WData =new wchar_t[8 +1];
            DataUI[16].WData[ 0] =0xe04;
            DataUI[16].WData[ 1] =0xe25;
            DataUI[16].WData[ 2] =0xe37;
            DataUI[16].WData[ 3] =0xe48;
            DataUI[16].WData[ 4] =0xe19;
            DataUI[16].WData[ 5] =0xe44;
            DataUI[16].WData[ 6] =0xe2a;
            DataUI[16].WData[ 7] =0xe49;
            DataUI[16].WData[ 8] =0;

            //------------------------------------------------------
            // Index 17
            //------------------------------------------------------
            DataUI[17].WData =new wchar_t[7 +1];
            DataUI[17].WData[ 0] =0xe01;
            DataUI[17].WData[ 1] =0xe25;
            DataUI[17].WData[ 2] =0xe31;
            DataUI[17].WData[ 3] =0xe49;
            DataUI[17].WData[ 4] =0xe19;
            DataUI[17].WData[ 5] =0xe43;
            DataUI[17].WData[ 6] =0xe08;
            DataUI[17].WData[ 7] =0;

            //------------------------------------------------------
            // Index 18
            //------------------------------------------------------
            DataUI[18].WData =new wchar_t[10 +1];
            DataUI[18].WData[ 0] =0xe01;
            DataUI[18].WData[ 1] =0xe25;
            DataUI[18].WData[ 2] =0xe31;
            DataUI[18].WData[ 3] =0xe48;
            DataUI[18].WData[ 4] =0xe19;
            DataUI[18].WData[ 5] =0xe41;
            DataUI[18].WData[ 6] =0xe01;
            DataUI[18].WData[ 7] =0xe25;
            DataUI[18].WData[ 8] =0xe49;
            DataUI[18].WData[ 9] =0xe07;
            DataUI[18].WData[10] =0;

            //------------------------------------------------------
            // Index 19
            //------------------------------------------------------
            DataUI[19].WData =new wchar_t[9 +1];
            DataUI[19].WData[ 0] =0xe04;
            DataUI[19].WData[ 1] =0xe23;
            DataUI[19].WData[ 2] =0xe37;
            DataUI[19].WData[ 3] =0xe49;
            DataUI[19].WData[ 4] =0xe19;
            DataUI[19].WData[ 5] =0xe40;
            DataUI[19].WData[ 6] =0xe04;
            DataUI[19].WData[ 7] =0xe23;
            DataUI[19].WData[ 8] =0xe07;
            DataUI[19].WData[ 9] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
