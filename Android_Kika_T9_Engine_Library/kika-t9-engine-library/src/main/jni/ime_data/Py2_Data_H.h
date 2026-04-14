#ifndef __Py2_DATA_UI_H_
#define __Py2_DATA_UI_H_

#define MAX_Py2_DATA_UI                    40

struct Py2_DATA_UI
{
       struct Py2_DATA_ID
       {
            wchar_t*          WData;
            wchar_t*          OData;
       };
       
       Py2_DATA_ID
       DataUI[MAX_Py2_DATA_UI];
       
       ~Py2_DATA_UI()
       {
            for ( int i =0; i <MAX_Py2_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       Py2_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[5 +1];
            DataUI[ 0].WData[ 0] =0x773c;
            DataUI[ 0].WData[ 1] =0x3a;
            DataUI[ 0].WData[ 2] =0x79;
            DataUI[ 0].WData[ 3] =0x61;
            DataUI[ 0].WData[ 4] =0x6e;
            DataUI[ 0].WData[ 5] =0;

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[5 +1];
            DataUI[ 1].WData[ 0] =0x70df;
            DataUI[ 1].WData[ 1] =0x3a;
            DataUI[ 1].WData[ 2] =0x79;
            DataUI[ 1].WData[ 3] =0x61;
            DataUI[ 1].WData[ 4] =0x6e;
            DataUI[ 1].WData[ 5] =0;

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[5 +1];
            DataUI[ 2].WData[ 0] =0x6f14;
            DataUI[ 2].WData[ 1] =0x3a;
            DataUI[ 2].WData[ 2] =0x79;
            DataUI[ 2].WData[ 3] =0x61;
            DataUI[ 2].WData[ 4] =0x6e;
            DataUI[ 2].WData[ 5] =0;

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[5 +1];
            DataUI[ 3].WData[ 0] =0x8a00;
            DataUI[ 3].WData[ 1] =0x3a;
            DataUI[ 3].WData[ 2] =0x79;
            DataUI[ 3].WData[ 3] =0x61;
            DataUI[ 3].WData[ 4] =0x6e;
            DataUI[ 3].WData[ 5] =0;

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[5 +1];
            DataUI[ 4].WData[ 0] =0x4e25;
            DataUI[ 4].WData[ 1] =0x3a;
            DataUI[ 4].WData[ 2] =0x79;
            DataUI[ 4].WData[ 3] =0x61;
            DataUI[ 4].WData[ 4] =0x6e;
            DataUI[ 4].WData[ 5] =0;

            //------------------------------------------------------
            // Index 5
            //------------------------------------------------------
            DataUI[ 5].WData =new wchar_t[5 +1];
            DataUI[ 5].WData[ 0] =0x71d5;
            DataUI[ 5].WData[ 1] =0x3a;
            DataUI[ 5].WData[ 2] =0x79;
            DataUI[ 5].WData[ 3] =0x61;
            DataUI[ 5].WData[ 4] =0x6e;
            DataUI[ 5].WData[ 5] =0;

            //------------------------------------------------------
            // Index 6
            //------------------------------------------------------
            DataUI[ 6].WData =new wchar_t[5 +1];
            DataUI[ 6].WData[ 0] =0x76d0;
            DataUI[ 6].WData[ 1] =0x3a;
            DataUI[ 6].WData[ 2] =0x79;
            DataUI[ 6].WData[ 3] =0x61;
            DataUI[ 6].WData[ 4] =0x6e;
            DataUI[ 6].WData[ 5] =0;

            //------------------------------------------------------
            // Index 7
            //------------------------------------------------------
            DataUI[ 7].WData =new wchar_t[5 +1];
            DataUI[ 7].WData[ 0] =0x5ca9;
            DataUI[ 7].WData[ 1] =0x3a;
            DataUI[ 7].WData[ 2] =0x79;
            DataUI[ 7].WData[ 3] =0x61;
            DataUI[ 7].WData[ 4] =0x6e;
            DataUI[ 7].WData[ 5] =0;

            //------------------------------------------------------
            // Index 8
            //------------------------------------------------------
            DataUI[ 8].WData =new wchar_t[5 +1];
            DataUI[ 8].WData[ 0] =0x6cbf;
            DataUI[ 8].WData[ 1] =0x3a;
            DataUI[ 8].WData[ 2] =0x79;
            DataUI[ 8].WData[ 3] =0x61;
            DataUI[ 8].WData[ 4] =0x6e;
            DataUI[ 8].WData[ 5] =0;

            //------------------------------------------------------
            // Index 9
            //------------------------------------------------------
            DataUI[ 9].WData =new wchar_t[5 +1];
            DataUI[ 9].WData[ 0] =0x989c;
            DataUI[ 9].WData[ 1] =0x3a;
            DataUI[ 9].WData[ 2] =0x79;
            DataUI[ 9].WData[ 3] =0x61;
            DataUI[ 9].WData[ 4] =0x6e;
            DataUI[ 9].WData[ 5] =0;

            //------------------------------------------------------
            // Index 10
            //------------------------------------------------------
            DataUI[10].WData =new wchar_t[5 +1];
            DataUI[10].WData[ 0] =0x708e;
            DataUI[10].WData[ 1] =0x3a;
            DataUI[10].WData[ 2] =0x79;
            DataUI[10].WData[ 3] =0x61;
            DataUI[10].WData[ 4] =0x6e;
            DataUI[10].WData[ 5] =0;

            //------------------------------------------------------
            // Index 11
            //------------------------------------------------------
            DataUI[11].WData =new wchar_t[5 +1];
            DataUI[11].WData[ 0] =0x8273;
            DataUI[11].WData[ 1] =0x3a;
            DataUI[11].WData[ 2] =0x79;
            DataUI[11].WData[ 3] =0x61;
            DataUI[11].WData[ 4] =0x6e;
            DataUI[11].WData[ 5] =0;

            //------------------------------------------------------
            // Index 12
            //------------------------------------------------------
            DataUI[12].WData =new wchar_t[5 +1];
            DataUI[12].WData[ 0] =0x7814;
            DataUI[12].WData[ 1] =0x3a;
            DataUI[12].WData[ 2] =0x79;
            DataUI[12].WData[ 3] =0x61;
            DataUI[12].WData[ 4] =0x6e;
            DataUI[12].WData[ 5] =0;

            //------------------------------------------------------
            // Index 13
            //------------------------------------------------------
            DataUI[13].WData =new wchar_t[5 +1];
            DataUI[13].WData[ 0] =0x5ef6;
            DataUI[13].WData[ 1] =0x3a;
            DataUI[13].WData[ 2] =0x79;
            DataUI[13].WData[ 3] =0x61;
            DataUI[13].WData[ 4] =0x6e;
            DataUI[13].WData[ 5] =0;

            //------------------------------------------------------
            // Index 14
            //------------------------------------------------------
            DataUI[14].WData =new wchar_t[5 +1];
            DataUI[14].WData[ 0] =0x598d;
            DataUI[14].WData[ 1] =0x3a;
            DataUI[14].WData[ 2] =0x79;
            DataUI[14].WData[ 3] =0x61;
            DataUI[14].WData[ 4] =0x6e;
            DataUI[14].WData[ 5] =0;

            //------------------------------------------------------
            // Index 15
            //------------------------------------------------------
            DataUI[15].WData =new wchar_t[5 +1];
            DataUI[15].WData[ 0] =0x5f66;
            DataUI[15].WData[ 1] =0x3a;
            DataUI[15].WData[ 2] =0x79;
            DataUI[15].WData[ 3] =0x61;
            DataUI[15].WData[ 4] =0x6e;
            DataUI[15].WData[ 5] =0;

            //------------------------------------------------------
            // Index 16
            //------------------------------------------------------
            DataUI[16].WData =new wchar_t[5 +1];
            DataUI[16].WData[ 0] =0x96c1;
            DataUI[16].WData[ 1] =0x3a;
            DataUI[16].WData[ 2] =0x79;
            DataUI[16].WData[ 3] =0x61;
            DataUI[16].WData[ 4] =0x6e;
            DataUI[16].WData[ 5] =0;

            //------------------------------------------------------
            // Index 17
            //------------------------------------------------------
            DataUI[17].WData =new wchar_t[5 +1];
            DataUI[17].WData[ 0] =0x9a8c;
            DataUI[17].WData[ 1] =0x3a;
            DataUI[17].WData[ 2] =0x79;
            DataUI[17].WData[ 3] =0x61;
            DataUI[17].WData[ 4] =0x6e;
            DataUI[17].WData[ 5] =0;

            //------------------------------------------------------
            // Index 18
            //------------------------------------------------------
            DataUI[18].WData =new wchar_t[5 +1];
            DataUI[18].WData[ 0] =0x7109;
            DataUI[18].WData[ 1] =0x3a;
            DataUI[18].WData[ 2] =0x79;
            DataUI[18].WData[ 3] =0x61;
            DataUI[18].WData[ 4] =0x6e;
            DataUI[18].WData[ 5] =0;

            //------------------------------------------------------
            // Index 19
            //------------------------------------------------------
            DataUI[19].WData =new wchar_t[5 +1];
            DataUI[19].WData[ 0] =0x960e;
            DataUI[19].WData[ 1] =0x3a;
            DataUI[19].WData[ 2] =0x79;
            DataUI[19].WData[ 3] =0x61;
            DataUI[19].WData[ 4] =0x6e;
            DataUI[19].WData[ 5] =0;

            //------------------------------------------------------
            // Index 20
            //------------------------------------------------------
            DataUI[20].WData =new wchar_t[5 +1];
            DataUI[20].WData[ 0] =0x54bd;
            DataUI[20].WData[ 1] =0x3a;
            DataUI[20].WData[ 2] =0x79;
            DataUI[20].WData[ 3] =0x61;
            DataUI[20].WData[ 4] =0x6e;
            DataUI[20].WData[ 5] =0;

            //------------------------------------------------------
            // Index 21
            //------------------------------------------------------
            DataUI[21].WData =new wchar_t[5 +1];
            DataUI[21].WData[ 0] =0x5bb4;
            DataUI[21].WData[ 1] =0x3a;
            DataUI[21].WData[ 2] =0x79;
            DataUI[21].WData[ 3] =0x61;
            DataUI[21].WData[ 4] =0x6e;
            DataUI[21].WData[ 5] =0;

            //------------------------------------------------------
            // Index 22
            //------------------------------------------------------
            DataUI[22].WData =new wchar_t[5 +1];
            DataUI[22].WData[ 0] =0x95eb;
            DataUI[22].WData[ 1] =0x3a;
            DataUI[22].WData[ 2] =0x79;
            DataUI[22].WData[ 3] =0x61;
            DataUI[22].WData[ 4] =0x6e;
            DataUI[22].WData[ 5] =0;

            //------------------------------------------------------
            // Index 23
            //------------------------------------------------------
            DataUI[23].WData =new wchar_t[5 +1];
            DataUI[23].WData[ 0] =0x63a9;
            DataUI[23].WData[ 1] =0x3a;
            DataUI[23].WData[ 2] =0x79;
            DataUI[23].WData[ 3] =0x61;
            DataUI[23].WData[ 4] =0x6e;
            DataUI[23].WData[ 5] =0;

            //------------------------------------------------------
            // Index 24
            //------------------------------------------------------
            DataUI[24].WData =new wchar_t[5 +1];
            DataUI[24].WData[ 0] =0x538c;
            DataUI[24].WData[ 1] =0x3a;
            DataUI[24].WData[ 2] =0x79;
            DataUI[24].WData[ 3] =0x61;
            DataUI[24].WData[ 4] =0x6e;
            DataUI[24].WData[ 5] =0;

            //------------------------------------------------------
            // Index 25
            //------------------------------------------------------
            DataUI[25].WData =new wchar_t[5 +1];
            DataUI[25].WData[ 0] =0x7130;
            DataUI[25].WData[ 1] =0x3a;
            DataUI[25].WData[ 2] =0x79;
            DataUI[25].WData[ 3] =0x61;
            DataUI[25].WData[ 4] =0x6e;
            DataUI[25].WData[ 5] =0;

            //------------------------------------------------------
            // Index 26
            //------------------------------------------------------
            DataUI[26].WData =new wchar_t[5 +1];
            DataUI[26].WData[ 0] =0x664f;
            DataUI[26].WData[ 1] =0x3a;
            DataUI[26].WData[ 2] =0x79;
            DataUI[26].WData[ 3] =0x61;
            DataUI[26].WData[ 4] =0x6e;
            DataUI[26].WData[ 5] =0;

            //------------------------------------------------------
            // Index 27
            //------------------------------------------------------
            DataUI[27].WData =new wchar_t[5 +1];
            DataUI[27].WData[ 0] =0x5ae3;
            DataUI[27].WData[ 1] =0x3a;
            DataUI[27].WData[ 2] =0x79;
            DataUI[27].WData[ 3] =0x61;
            DataUI[27].WData[ 4] =0x6e;
            DataUI[27].WData[ 5] =0;

            //------------------------------------------------------
            // Index 28
            //------------------------------------------------------
            DataUI[28].WData =new wchar_t[5 +1];
            DataUI[28].WData[ 0] =0x6df9;
            DataUI[28].WData[ 1] =0x3a;
            DataUI[28].WData[ 2] =0x79;
            DataUI[28].WData[ 3] =0x61;
            DataUI[28].WData[ 4] =0x6e;
            DataUI[28].WData[ 5] =0;

            //------------------------------------------------------
            // Index 29
            //------------------------------------------------------
            DataUI[29].WData =new wchar_t[5 +1];
            DataUI[29].WData[ 0] =0x884d;
            DataUI[29].WData[ 1] =0x3a;
            DataUI[29].WData[ 2] =0x79;
            DataUI[29].WData[ 3] =0x61;
            DataUI[29].WData[ 4] =0x6e;
            DataUI[29].WData[ 5] =0;

            //------------------------------------------------------
            // Index 30
            //------------------------------------------------------
            DataUI[30].WData =new wchar_t[5 +1];
            DataUI[30].WData[ 0] =0x781a;
            DataUI[30].WData[ 1] =0x3a;
            DataUI[30].WData[ 2] =0x79;
            DataUI[30].WData[ 3] =0x61;
            DataUI[30].WData[ 4] =0x6e;
            DataUI[30].WData[ 5] =0;

            //------------------------------------------------------
            // Index 31
            //------------------------------------------------------
            DataUI[31].WData =new wchar_t[5 +1];
            DataUI[31].WData[ 0] =0x7131;
            DataUI[31].WData[ 1] =0x3a;
            DataUI[31].WData[ 2] =0x79;
            DataUI[31].WData[ 3] =0x61;
            DataUI[31].WData[ 4] =0x6e;
            DataUI[31].WData[ 5] =0;

            //------------------------------------------------------
            // Index 32
            //------------------------------------------------------
            DataUI[32].WData =new wchar_t[5 +1];
            DataUI[32].WData[ 0] =0x5830;
            DataUI[32].WData[ 1] =0x3a;
            DataUI[32].WData[ 2] =0x79;
            DataUI[32].WData[ 3] =0x61;
            DataUI[32].WData[ 4] =0x6e;
            DataUI[32].WData[ 5] =0;

            //------------------------------------------------------
            // Index 33
            //------------------------------------------------------
            DataUI[33].WData =new wchar_t[5 +1];
            DataUI[33].WData[ 0] =0x6a90;
            DataUI[33].WData[ 1] =0x3a;
            DataUI[33].WData[ 2] =0x79;
            DataUI[33].WData[ 3] =0x61;
            DataUI[33].WData[ 4] =0x6e;
            DataUI[33].WData[ 5] =0;

            //------------------------------------------------------
            // Index 34
            //------------------------------------------------------
            DataUI[34].WData =new wchar_t[5 +1];
            DataUI[34].WData[ 0] =0x9609;
            DataUI[34].WData[ 1] =0x3a;
            DataUI[34].WData[ 2] =0x79;
            DataUI[34].WData[ 3] =0x61;
            DataUI[34].WData[ 4] =0x6e;
            DataUI[34].WData[ 5] =0;

            //------------------------------------------------------
            // Index 35
            //------------------------------------------------------
            DataUI[35].WData =new wchar_t[5 +1];
            DataUI[35].WData[ 0] =0x7430;
            DataUI[35].WData[ 1] =0x3a;
            DataUI[35].WData[ 2] =0x79;
            DataUI[35].WData[ 3] =0x61;
            DataUI[35].WData[ 4] =0x6e;
            DataUI[35].WData[ 5] =0;

            //------------------------------------------------------
            // Index 36
            //------------------------------------------------------
            DataUI[36].WData =new wchar_t[5 +1];
            DataUI[36].WData[ 0] =0x9122;
            DataUI[36].WData[ 1] =0x3a;
            DataUI[36].WData[ 2] =0x79;
            DataUI[36].WData[ 3] =0x61;
            DataUI[36].WData[ 4] =0x6e;
            DataUI[36].WData[ 5] =0;

            //------------------------------------------------------
            // Index 37
            //------------------------------------------------------
            DataUI[37].WData =new wchar_t[5 +1];
            DataUI[37].WData[ 0] =0x5186;
            DataUI[37].WData[ 1] =0x3a;
            DataUI[37].WData[ 2] =0x79;
            DataUI[37].WData[ 3] =0x61;
            DataUI[37].WData[ 4] =0x6e;
            DataUI[37].WData[ 5] =0;

            //------------------------------------------------------
            // Index 38
            //------------------------------------------------------
            DataUI[38].WData =new wchar_t[5 +1];
            DataUI[38].WData[ 0] =0x7b75;
            DataUI[38].WData[ 1] =0x3a;
            DataUI[38].WData[ 2] =0x79;
            DataUI[38].WData[ 3] =0x61;
            DataUI[38].WData[ 4] =0x6e;
            DataUI[38].WData[ 5] =0;

            //------------------------------------------------------
            // Index 39
            //------------------------------------------------------
            DataUI[39].WData =new wchar_t[5 +1];
            DataUI[39].WData[ 0] =0x814c;
            DataUI[39].WData[ 1] =0x3a;
            DataUI[39].WData[ 2] =0x79;
            DataUI[39].WData[ 3] =0x61;
            DataUI[39].WData[ 4] =0x6e;
            DataUI[39].WData[ 5] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
