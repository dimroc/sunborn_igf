xof 0303txt 0032
template AnimTicksPerSecond {
 <9e415a43-7ba6-4a73-8743-b73d47e88476>
 DWORD AnimTicksPerSecond;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Animation {
 <3d82ab4f-62da-11cf-ab39-0020af71e433>
 [...]
}

template AnimationSet {
 <3d82ab50-62da-11cf-ab39-0020af71e433>
 [Animation <3d82ab4f-62da-11cf-ab39-0020af71e433>]
}

template FloatKeys {
 <10dd46a9-775b-11cf-8f52-0040333594a3>
 DWORD nValues;
 array FLOAT values[nValues];
}

template TimedFloatKeys {
 <f406b180-7b3b-11cf-8f52-0040333594a3>
 DWORD time;
 FloatKeys tfkeys;
}

template AnimationKey {
 <10dd46a8-775b-11cf-8f52-0040333594a3>
 DWORD keyType;
 DWORD nKeys;
 array TimedFloatKeys keys[nKeys];
}


AnimTicksPerSecond {
 24;
}

Frame DXCC_ROOT {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame persp {
  

  FrameTransformMatrix {
   0.990748,0.000000,0.135716,0.000000,0.000620,0.999990,-0.004524,0.000000,-0.135714,0.004567,0.990737,0.000000,13.926530,-0.468608,-101.666145,1.000000;;
  }

  Frame perspShape {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }

 Frame top {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,0.000000,100.000000,0.000000,1.000000;;
  }

  Frame topShape {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }

 Frame front {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,-100.000000,1.000000;;
  }

  Frame frontShape {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }

 Frame side {
  

  FrameTransformMatrix {
   0.000000,0.000000,1.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,0.000000,100.000000,0.000000,0.000000,1.000000;;
  }

  Frame sideShape {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }
  }
 }

 Frame Reference {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Frame locatorShape1 {
   

   FrameTransformMatrix {
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
   }
  }

  Frame hips {
   

   FrameTransformMatrix {
    0.482416,0.860012,-0.209502,0.000000,-0.286448,-0.077304,-0.976934,0.000000,-0.803261,0.498350,0.196091,0.000000,0.125026,0.346117,-0.471567,1.000000;;
   }

   Frame spine {
    

    FrameTransformMatrix {
     1.000536,0.126386,0.057570,0.000000,-0.133195,1.009275,0.099149,0.000000,-0.042455,-0.099558,0.956409,0.000000,1.000223,0.006206,-0.022410,1.000000;;
    }

    Frame spine1 {
     

     FrameTransformMatrix {
      0.954081,-0.166908,-0.294352,0.000000,0.188787,1.003472,0.042909,0.000000,0.265185,-0.088798,0.909892,0.000000,2.000057,0.022571,-0.034008,1.000000;;
     }

     Frame spine2 {
      

      FrameTransformMatrix {
       0.974280,-0.199434,-0.269463,0.000000,0.195609,1.000370,-0.033139,0.000000,0.248061,-0.018344,0.910477,0.000000,2.999515,0.014201,-0.053970,1.000000;;
      }

      Frame spine3 {
       

       FrameTransformMatrix {
        0.970714,-0.164893,-0.338752,0.000000,0.311519,0.031509,0.877340,0.000000,-0.140527,-1.003845,0.085949,0.000000,2.348393,0.001847,-0.033931,1.000000;;
       }

       Frame neck {
        

        FrameTransformMatrix {
         0.720218,0.633109,-0.317561,0.000000,0.452294,-0.733008,-0.435582,0.000000,-0.528936,0.176903,-0.846926,0.000000,2.009976,0.004825,-0.000724,1.000000;;
        }

        Frame head1 {
         

         FrameTransformMatrix {
          0.925335,-0.120992,0.387004,0.000000,0.048172,-0.896481,-0.395453,0.000000,0.398733,0.388412,-0.831947,0.000000,0.623226,-0.001190,0.007161,1.000000;;
         }

         Frame head_end {
          

          FrameTransformMatrix {
           -0.003956,0.014732,0.999884,0.000000,0.965794,0.259312,0.000000,0.000000,-0.259282,0.965681,-0.015254,0.000000,3.134743,0.000000,0.000000,1.000000;;
          }
         }
        }
       }

       Frame LeftShoulder {
        

        FrameTransformMatrix {
         0.131766,-0.053061,1.022603,0.000000,1.019922,0.109112,-0.125759,0.000000,-0.091417,0.923308,0.059688,0.000000,0.450845,-0.197371,2.001875,1.000000;;
        }

        Frame LeftArm {
         

         FrameTransformMatrix {
          0.222368,-0.931642,-0.243172,0.000000,1.036278,0.222836,0.093892,0.000000,-0.029868,-0.244854,0.910776,0.000000,2.176714,0.018564,0.022048,1.000000;;
         }

         Frame LeftForearm {
          

          FrameTransformMatrix {
           0.071022,0.118240,-0.974944,0.000000,0.205049,1.041383,0.141235,0.000000,0.932743,-0.189752,0.044935,0.000000,5.005538,0.020508,0.082705,1.000000;;
          }

          Frame LeftHand {
           

           FrameTransformMatrix {
            0.981130,0.021729,0.040727,0.000000,-0.023673,1.071662,-0.001483,0.000000,-0.039707,0.000446,0.956322,0.000000,6.002777,-0.023209,-0.079277,1.000000;;
           }

           Frame LeftFingers {
            

            FrameTransformMatrix {
             1.000000,0.000000,0.000000,0.000000,0.000000,-1.000000,-0.000000,0.000000,0.000000,0.000000,-1.000000,0.000000,2.331300,-0.000000,-0.000000,1.000000;;
            }
           }
          }
         }
        }
       }

       Frame RightShoulder {
        

        FrameTransformMatrix {
         0.458685,-0.281121,-0.874858,0.000000,0.917584,0.196978,0.417791,0.000000,0.048176,-0.872945,0.305764,0.000000,0.453878,-0.234745,-1.999129,1.000000;;
        }

        Frame RightArm {
         

         FrameTransformMatrix {
          -0.673701,-0.204340,0.712004,0.000000,0.293896,0.838716,0.518791,0.000000,-0.662227,0.526223,-0.475579,0.000000,2.175749,0.003402,-0.018160,1.000000;;
         }

         Frame RightForearm {
          

          FrameTransformMatrix {
           0.344391,0.402414,0.865265,0.000000,-0.934400,0.027115,0.359297,0.000000,0.114115,-0.878293,0.363052,0.000000,5.003425,-0.026470,0.140463,1.000000;;
          }

          Frame RightHand {
           

           FrameTransformMatrix {
            0.972416,0.096719,-0.282099,0.000000,-0.086014,1.009740,0.049697,0.000000,0.270267,-0.022451,0.923931,0.000000,6.001526,-0.090427,0.012061,1.000000;;
           }

           Frame RightFingers {
            

            FrameTransformMatrix {
             -1.000000,0.000000,-0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,-1.000000,0.000000,2.331340,-0.000000,0.000000,1.000000;;
            }
           }
          }
         }
        }
       }
      }
     }
    }
   }

   Frame LeftUpLeg {
    

    FrameTransformMatrix {
     -0.281500,-0.553879,-0.817368,0.000000,0.794133,-0.565768,0.109887,0.000000,-0.530419,-0.626570,0.607263,0.000000,-1.011724,-1.799536,0.022852,1.000000;;
    }

    Frame LeftLeg {
     

     FrameTransformMatrix {
      0.268428,-0.966845,0.038063,0.000000,-0.765891,-0.190167,0.570762,0.000000,-0.570674,-0.191092,-0.829442,0.000000,7.073488,0.220594,0.051105,1.000000;;
     }

     Frame LeftFoot {
      

      FrameTransformMatrix {
       0.022618,-0.974538,-0.018604,0.000000,-0.952559,-0.028060,0.311779,0.000000,-0.319453,0.011199,-0.974997,0.000000,8.062625,-0.125728,0.114632,1.000000;;
      }

      Frame LeftToes {
       

       FrameTransformMatrix {
        0.000001,0.000000,1.000000,0.000000,-0.155641,0.987814,-0.000000,0.000000,-0.987814,-0.155641,0.000001,0.000000,2.670070,0.000000,0.000000,1.000000;;
       }
      }
     }
    }
   }

   Frame RightUpLeg {
    

    FrameTransformMatrix {
     -0.548937,0.390624,-0.755014,0.000000,0.811450,0.125691,-0.524940,0.000000,-0.113835,-0.930901,-0.398859,0.000000,-0.987978,1.787975,0.014143,1.000000;;
    }

    Frame RightLeg {
     

     FrameTransformMatrix {
      0.056952,-1.035914,0.024207,0.000000,-0.976194,-0.056809,-0.134410,0.000000,0.139555,-0.015856,-1.006859,0.000000,7.077843,0.201896,-0.108521,1.000000;;
     }

     Frame RightFoot {
      

      FrameTransformMatrix {
       0.152634,-0.952506,-0.000352,0.000000,-1.043698,-0.167222,-0.067197,0.000000,0.063859,0.010609,-1.018252,0.000000,8.064962,-0.053865,-0.060083,1.000000;;
      }

      Frame RightToes {
       

       FrameTransformMatrix {
        0.000001,0.000000,1.000000,0.000000,0.155651,-0.987812,0.000000,0.000000,0.987812,0.155651,-0.000002,0.000000,2.670073,0.000000,0.000000,1.000000;;
       }
      }
     }
    }
   }
  }
 }
}

AnimationSet Default {
 

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.997682,-0.002278,-0.068015,-0.000155;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;13.926530,-0.468608,-101.666145;;;
  }
  { persp }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
  { perspShape }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.707107,0.707107,0.000000,0.000000;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.000000,100.000000,0.000000;;;
  }
  { top }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
  { topShape }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.000000,0.000000,-100.000000;;;
  }
  { front }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
  { frontShape }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.707107,0.000000,-0.707107,0.000000;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;100.000000,0.000000,0.000000;;;
  }
  { side }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
  { sideShape }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
  { Reference }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
  { locatorShape1 }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.633630,-0.581194,-0.246284,0.447293;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.008085,1.020993,0.965418;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.125026,0.346117,-0.471567;;;
  }
  { hips }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.996355,0.050276,-0.025368,0.064068;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.010129,1.022843,0.962513;;;
  }

  AnimationKey {
   2;
   1;
   0;3;1.000223,0.006206,-0.022410;;;
  }
  { spine }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.984916,0.034336,0.144519,-0.088740;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.012310,1.021977,0.951899;;;
  }

  AnimationKey {
   2;
   1;
   0;3;2.000057,0.022571,-0.034008;;;
  }
  { spine1 }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.986298,-0.003310,0.132908,-0.097679;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.030342,1.019854,0.943843;;;
  }

  AnimationKey {
   2;
   1;
   0;3;2.999515,0.014201,-0.053970;;;
  }
  { spine2 }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.715989,0.673412,0.065359,-0.172060;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.041263,0.931537,1.017271;;;
  }

  AnimationKey {
   2;
   1;
   0;3;2.348393,0.001847,-0.033931;;;
  }
  { spine3 }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;0.172030,0.909340,0.301141,-0.229828;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.010142,0.965195,1.014076;;;
  }

  AnimationKey {
   2;
   1;
   0;3;2.009976,0.004825,-0.000724;;;
  }
  { neck }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;0.206741,0.956671,-0.018464,0.204199;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.010275,0.981011,1.000993;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.623226,-0.001190,0.007161;;;
  }
  { head1 }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.556799,-0.433586,-0.565359,-0.427021;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;3.134743,0.000000,0.000000;;;
  }
  { head_end }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.569520,-0.489348,-0.477953,-0.455792;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.032422,1.033422,0.929741;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.450845,-0.197371,2.001875;;;
  }
  { LeftShoulder }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.774542,0.112236,0.069209,-0.618626;;;
  }

  AnimationKey {
   1;
   1;
   0;3;0.988199,1.064116,0.943588;;;
  }

  AnimationKey {
   2;
   1;
   0;3;2.176714,0.018564,0.022048;;;
  }
  { LeftArm }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.723166,0.114439,0.680680,-0.024690;;;
  }

  AnimationKey {
   1;
   1;
   0;3;0.984652,1.070733,0.952908;;;
  }

  AnimationKey {
   2;
   1;
   0;3;5.005538,0.020508,0.082705;;;
  }
  { LeftForearm }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.999724,-0.000463,-0.020743,0.011055;;;
  }

  AnimationKey {
   1;
   1;
   0;3;0.982215,1.071924,0.957146;;;
  }

  AnimationKey {
   2;
   1;
   0;3;6.002777,-0.023209,-0.079277;;;
  }
  { LeftHand }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;0.000000,1.000000,0.000000,0.000000;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;2.331300,-0.000000,-0.000000;;;
  }
  { LeftFingers }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.701514,0.480817,0.322105,-0.415863;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.027034,1.027283,0.926199;;;
  }

  AnimationKey {
   2;
   1;
   0;3;0.453878,-0.234745,-1.999129;;;
  }
  { RightShoulder }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;0.403763,0.023619,0.862839,0.303194;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.001288,1.029060,0.970377;;;
  }

  AnimationKey {
   2;
   1;
   0;3;2.175749,0.003402,-0.018160;;;
  }
  { RightArm }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.660649,0.482986,-0.277634,0.503176;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.014508,1.001465,0.957198;;;
  }

  AnimationKey {
   2;
   1;
   0;3;5.003425,-0.026470,0.140463;;;
  }
  { RightForearm }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.988783,0.018279,0.141090,0.045477;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.017117,1.014615,0.962911;;;
  }

  AnimationKey {
   2;
   1;
   0;3;6.001526,-0.090427,0.012061;;;
  }
  { RightHand }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.000000,0.000000,1.000000,0.000000;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;2.331340,-0.000000,0.000000;;;
  }
  { RightFingers }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;0.431257,-0.420628,-0.160384,0.781900;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.026701,0.981232,1.021128;;;
  }

  AnimationKey {
   2;
   1;
   0;3;-1.011724,-1.799536,0.022852;;;
  }
  { LeftUpLeg }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.256258,0.753653,-0.580261,-0.172152;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.004137,0.973921,1.024773;;;
  }

  AnimationKey {
   2;
   1;
   0;3;7.073488,0.220594,0.051105;;;
  }
  { LeftLeg }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.106040,0.707357,-0.689031,-0.116780;;;
  }

  AnimationKey {
   1;
   1;
   0;3;0.974978,1.002677,1.026058;;;
  }

  AnimationKey {
   2;
   1;
   0;3;8.062625,-0.125728,0.114632;;;
  }
  { LeftFoot }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.704950,0.055196,-0.704949,0.055196;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;2.670070,0.000000,0.000000;;;
  }
  { LeftToes }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;-0.220862,0.424243,0.718125,-0.505503;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.011912,0.974583,1.019129;;;
  }

  AnimationKey {
   2;
   1;
   0;3;-0.987978,1.787975,0.014143;;;
  }
  { RightUpLeg }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;0.041575,0.725059,-0.685196,0.055375;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.037761,0.987040,1.016608;;;
  }

  AnimationKey {
   2;
   1;
   0;3;7.077843,0.201896,-0.108521;;;
  }
  { RightLeg }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;0.024271,0.760607,-0.648436,0.020452;;;
  }

  AnimationKey {
   1;
   1;
   0;3;0.964658,1.059143,1.020308;;;
  }

  AnimationKey {
   2;
   1;
   0;3;8.064962,-0.053865,-0.060083;;;
  }
  { RightFoot }
 }

 Animation {
  

  AnimationKey {
   0;
   1;
   0;4;0.055199,0.704949,0.055199,0.704948;;;
  }

  AnimationKey {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey {
   2;
   1;
   0;3;2.670073,0.000000,0.000000;;;
  }
  { RightToes }
 }
}