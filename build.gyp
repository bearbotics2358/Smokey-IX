{
  'make_global_settings': [
    ['CC', '/usr/bin/arm-frc-linux-gnueabi-gcc'],
    ['CXX', '/usr/bin/arm-frc-linux-gnueabi-g++'],
  ],
  'targets': [
    {
      'target_name': 'FRCUserProgram',
      'type': 'executable',
      'cflags': [
        '-std=c++1y'
      ],
      'include_dirs': [
        'src'
      ],
      'libraries': [
        '-lwpi',
      ],
      'sources': [
        'src/JrimmyGyro.cpp',
        'src/Lifter.cpp',
        'src/PivotArm.cpp',
        'src/Roller.cpp',
        'src/ShifterController.cpp',
        'src/Shooter.cpp',
        'src/SmokeyIX.cpp',
        'src/Tank.cpp',
        'src/Vision/AxisImageSource.cpp',
        'src/Vision/ImageFilter.cpp',
        'src/Vision/SimpleFilters.cpp',
        'src/Vision/StillImageSource.cpp',
        'src/Vision/TargetDetector.cpp'
      ],
      'conditions': [
        ['OS=="linux"', {
          'include_dirs': [
            '~/wpilib/cpp/current/include'
          ],
          'libraries': [
            '-L~/wpilib/cpp/current/lib',
            '-Wl,-rpath,/opt/GenICam_v2_3/bin/Linux_armv7-a'
          ]
        }]
      ]
    }
  ]
}
