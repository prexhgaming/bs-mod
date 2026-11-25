# Messiah Engine Python SDK

class A lock object is a synchronization primitive:
    To create a lock,
call threading.Lock().  Methods are:

acquire() -- lock the lock, possibly blocking until it can be obtained
release() -- unlock of the lock
locked() -- test whether the lock is currently locked

A lock is not owned by the thread that locked it; another thread may
unlock it.  A thread attempting to lock a lock that it has already locked
will block until another thread unlocks it.  Deadlocks may ensue. = 0x6D48F3FAD0

class ActionComponent:
    Channel = 0x8C
    Delay = 0x84
    EnterWorldPlay = 0x8F
    Identifier = 0x68
    IsPlaying = 0x90
    RemoveMotion = 0x8E
    SinglePlay = 0x8D
    Speed = 0x80
    StartTime = 0x88

class ActorComponent:
    ApplyMotion = 0x1E0
    GraphPath = 0x270
    PhySpace = 0x2C0
    PoseModifier = 0x288
    RigidBodyAttachments = 0x290
    TickMode = 0x1E4
    UserTag = 0x1C8
    UserVelocity = 0x1B8

class AeroplaneFilter:
    ApplyMotion = 0x80
    DebugOutput = 0x81
    DriveRB = 0x118
    InterpThreshold = 0x88
    TimeErrorTH = 0x84
    World = 0xE8

class AimPlacer:
    AimDirection = 0xF8
    AimDirectionPitchMax = 0x104
    AimDirectionPitchMin = 0x108
    Direction = 0xEC
    EnableFocusInertia = 0xBD
    EnableInertia = 0xBC
    ExpRatio = 0xB8
    Finished = 0xA0
    FocusInertiaCoeff = 0xC0
    FocusInertiaMaxLength = 0xC4
    FocusPos = 0xC8
    Fov = 0xA4
    InertiaCoeff = 0xB0
    InertiaTol = 0xAC
    InertiaType = 0xA8
    ManualAdjust = 0x94
    MaxYawDelta = 0xB4
    PitchMax = 0x9C
    PitchMin = 0x98
    Target = 0x110
    TargetPos = 0xD4
    TargetPosOffset = 0xE0
    TargetTransform = 0x118

class AnimatedController:
    CustomAspect = 0xB8
    FieldOfView = 0xD4
    IsPlaying = 0xA8
    PauseEnd = 0xA9
    Pitch = 0xC8
    PlayedTime = 0xAC
    Playrate = 0xB0
    Position = 0xBC
    Roll = 0xD0
    VerticalFOV = 0xB4
    Yaw = 0xCC

class AnimatedWebP:
    CacheCount = 0xA8
    GifGuid = 0xB0
    Height = 0xC4
    TimeScale = 0xC8
    Width = 0xC0

class AnimatorDestructor:
    BreakingTime = 0x64
    DisapperedAfterBroken = 0x68
    IsBroken = 0x69

class ApplicationInfo:
    AppPath = 0x18
    AssetPath = 0x30
    LocalPath = 0x48

class AreaLightComponent:
    DoubleSide = 0x61
    Enable = 0x60
    GIOnly = 0x78
    Intensity = 0x70
    LightColor = 0x64
    Range = 0x74

class AudioComponent:
    AudioShape = 0x1F9
    AudioType = 0x1F8
    ControlPoints = 0x200
    Direction = 0x228
    EventID = 0x198
    EventParam = 0x1C8
    ExtraEventIDs = 0x1A8
    ExtraEventParams = 0x1E0
    FadeOutTime = 0x21C
    FadeOutType = 0x220
    FollowListener = 0x222
    GameObjectID = 0x1A0
    IsDynamic = 0x223
    IsInVolume = 0x160
    IsSpatial = 0x224
    LeaveWithoutStop = 0x221
    LocalBound = 0x164
    OneShot = 0x225
    PolygonRampDistance = 0x218
    Sound = 0x158
    UpToOne = 0x226
    Version = 0x1C0
    WorldBound = 0x17C

class AudioObjectComponent:
    GameObjectID = 0x68
    GameObjectName = 0x70

class AudioObstructionComponent:
    EnableDiffraction = 0x228
    EnableDiffractionOnBoundary = 0x229
    GeometryID = 0x208
    Occlusion = 0x22C
    Room = 0x210
    Sound = 0x200
    Texture = 0x230

class AudioPortalComponent:
    Enabled = 0x250
    PortalID = 0x218
    RoomB = 0x238
    RoomF = 0x220
    Sound = 0x210

class AudioRoomComponent:
    AuxSend = 0x230
    EnableDiffraction = 0x235
    EnableDiffractionOnBoundary = 0x236
    EnableObstruction = 0x234
    GeometryID = 0x200
    Index = 0x1E8
    Occlusion = 0x238
    Priority = 0x208
    ReverbAuxBus = 0x210
    ReverbLevel = 0x228
    RoomID = 0x1F8
    Sound = 0x1F0
    Texture = 0x240
    WallOcclusion = 0x22C

class Auro 10:
    1 = 0x1

class Auro 11:
    1 = 0x1
    1 (7+4) = 0x1

class Auro 13:
    1 = 0x1
    1 (7+5+1) = 0x1

class Auro 8:
    0 = 0x1

class Auro 9:
    1 = 0x1

class AuxiliaryMapSlot:
    Texture = 0x18

class BZ2Decompressor()
--

Create a decompressor object for decompressing data incrementally:
    For one-shot decompression, use the decompress() function instead. = 0x130

class BaseEnvVolumeComponent:
    AOOffset = 0x398
    AOScale = 0x39C
    Absorption = 0x7C4
    AbsorptionScale = 0x7C0
    AerialPerspectiveStartDepth = 0x7F8
    AerialPespectiveViewDistanceScale = 0x7EC
    ApplyTemperature = 0x310
    AtmosphereHeight = 0x778
    AtmosphereHorizonHeight = 0x77C
    AtmosphereMultiScattering = 0x780
    AutoExposure = 0x628
    AutoExposureBias = 0x670
    AutoExposureBiasCurve = 0x678
    AutoExposureMaxEV100 = 0x694
    AutoExposureMinEV100 = 0x690
    AutoExposureSpeedDown = 0x69C
    AutoExposureSpeedUp = 0x698
    BaseSkyMap = 0x1E8
    BloomDelta = 0x658
    BloomDirtMaskIntensity = 0x6CC
    BloomDirtMaskTint = 0x6C0
    BloomHack = 0x66C
    BloomIntensity = 0x668
    BloomLayerStrength1 = 0x6D0
    BloomLayerStrength2 = 0x6D4
    BloomLayerStrength3 = 0x6D8
    BloomLayerStrength4 = 0x6DC
    BloomLayerStrength5 = 0x6E0
    BloomLevel = 0x654
    BloomScale = 0x3DC
    BloomThreshold = 0x6BC
    BloomTint = 0x65C
    BrightDelta = 0x648
    BrightThreshold = 0x644
    CAIntensity = 0x268
    CAStartOffset = 0x26C
    CGGamma = 0x298
    CameraISO = 0x6E8
    CameraShutterSpeed = 0x6E4
    CandelaAlbedoBias = 0x3AC
    CandelaAlbedoScale = 0x3B0
    CandelaProbeLightBias = 0x3A8
    CandelaSkyLightingScale = 0x3A4
    CloudAmbientColor = 0x464
    CloudAmbientIntensity = 0x470
    CloudAtmosphereInScatteringScale = 0x474
    CloudAtmosphereSkyLightScale = 0x478
    CloudCoverage = 0x48C
    CloudCoverageUVOffsetU = 0x49C
    CloudCoverageUVOffsetV = 0x4A0
    CloudCutoff = 0x488
    CloudDarkness = 0x4B0
    CloudDensityScale = 0x4AC
    CloudDetailNoiseMapRepeat = 0x4A8
    CloudFadeDistance = 0x4CC
    CloudGodRayBlurDistance = 0x4E0
    CloudGodRayColor = 0x4D0
    CloudGodRayRange = 0x4DC
    CloudMaxHeight = 0x4C0
    CloudMaxTraceDistance = 0x4C4
    CloudMinHeight = 0x4BC
    CloudNoiseMapRepeat = 0x4A4
    CloudPhaseG = 0x47C
    CloudSilverIntensity = 0x480
    CloudSilverSpread = 0x484
    CloudSunColor = 0x458
    CloudTypeOffset = 0x4C8
    CloudWindSpeed = 0x4B8
    CloudWindYaw = 0x4B4
    ColorGradingMap = 0x2C8
    ColorGradingMapHDR = 0x2D8
    ColorGradingMapLOG = 0x2E8
    Contrast = 0x288
    ContrastLuminance = 0x294
    CustomParameters = 0x958
    CustomParams1 = 0x944
    DepthOfFieldFstop = 0x6EC
    DetailEnhancementLevel = 0x64C
    DiyParam0 = 0x358
    DiyParam1 = 0x368
    DiyParam2 = 0x378
    DiyParam3 = 0x388
    EnableTimeOfDay = 0x8E8
    ExposureCompensation = 0x634
    EyeAdaptionLevel = 0x62C
    EyeAdaptionScale = 0x630
    FixAdaptionLum = 0x640
    FogBaseHeight = 0x72C
    FogColor = 0x710
    FogColorIntensity = 0x71C
    FogDensity = 0x720
    FogHeight = 0x724
    FogMinTransmittance = 0x730
    FogOffset = 0x728
    G = 0x3D4
    Gain = 0x2A8
    GainLuminance = 0x2B4
    Gamma = 0x3C8
    GammaLuminance = 0x2A4
    GodRayBrightLevel = 0x708
    GodRayDarkLevel = 0x70C
    GodRayIntensity = 0x704
    GodRayLength = 0x6F0
    GodRayOccludeDistance = 0x6F4
    GodRayTint = 0x6F8
    GroundAlbedo = 0x76C
    GroundRadius = 0x768
    HdrScale = 0x3C4
    HistogramHighPercent = 0x6A4
    HistogramLowPercent = 0x6A0
    HistogramMaxEV100 = 0x6AC
    HistogramMinEV100 = 0x6A8
    HorizonColor = 0x3E0
    HorizonIntensity = 0x3EC
    KMie = 0x3D0
    KRayleigh = 0x3CC
    KeyFrame = 0x1C8
    LatitudeMoon = 0x404
    LensFlareRate = 0x33C
    LensFlareTintColor = 0x340
    LightMapScale = 0x3A0
    LitMapGiScale = 0x1CC
    LocalExposureHighLight = 0x6B4
    LocalExposureShadow = 0x6B0
    LocalExposureSigma = 0x6B8
    Longitude = 0x2FC
    LongitudeMoon = 0x408
    MieAbsorption = 0x7AC
    MieAbsorptionScale = 0x7A8
    MieAnisotropy = 0x7B8
    MieExponentialDistribution = 0x7BC
    MieScattering = 0x79C
    MieScatteringScale = 0x798
    MoonBlend = 0x41C
    MoonColor = 0x40C
    MoonColorIntensity = 0x418
    MoonInnerColor = 0x420
    MoonInnerColorIntensity = 0x42C
    MoonInnerScale = 0x448
    MoonOuterColor = 0x430
    MoonOuterColorIntensity = 0x43C
    MoonOuterScale = 0x444
    MoonSize = 0x440
    MoonriseTime = 0x8F4
    MoonsetTime = 0x8F8
    Offset = 0x2B8
    OffsetLuminance = 0x2C4
    Pitch = 0x34C
    RainIntensity = 0x21C
    RainNormalScale = 0x220
    RainPeriod = 0x218
    RainRate = 0x200
    RayleighExponentialDistribution = 0x794
    RayleighScattering = 0x788
    RayleighScatteringScale = 0x784
    ReflectionDir = 0x830
    ReflectionPlane = 0x834
    ReflectionScale = 0x1D0
    ReflectionZFar = 0x838
    Roll = 0x354
    SGPostProcessMaterials = 0x900
    SSPRParams = 0x928
    SSPRZenSpecColor = 0x938
    SSSBlurWidth = 0x7FC
    SSSFalloffB = 0x808
    SSSFalloffG = 0x804
    SSSFalloffR = 0x800
    SSSStrengthB = 0x814
    SSSStrengthG = 0x810
    SSSStrengthR = 0x80C
    Saturation = 0x278
    SaturationLuminance = 0x284
    SecondsPerDay = 0x850
    SelfShadowRate = 0x828
    ShadowBias = 0x81C
    ShadowFilterRange = 0x824
    ShadowRate = 0x818
    ShadowSlopeBias = 0x820
    ShadowZFarScale = 0x82C
    SkyCustom1 = 0x518
    SkyCustom2 = 0x558
    SkyCustom3 = 0x598
    SkyCustom4 = 0x5D8
    SkyCustom5 = 0x618
    SkyHDR1 = 0x514
    SkyHDR2 = 0x554
    SkyHDR3 = 0x594
    SkyHDR4 = 0x5D4
    SkyHDR5 = 0x614
    SkyLuminanceFactor = 0x7DC
    SkyLuminanceScale = 0x7E8
    SkyMapBeginU = 0x1F8
    SkyMapBeginU1 = 0x500
    SkyMapBeginU2 = 0x540
    SkyMapBeginU3 = 0x580
    SkyMapBeginU4 = 0x5C0
    SkyMapBeginU5 = 0x600
    SkyMapBeginV1 = 0x508
    SkyMapBeginV2 = 0x548
    SkyMapBeginV3 = 0x588
    SkyMapBeginV4 = 0x5C8
    SkyMapBeginV5 = 0x608
    SkyMapLayer1 = 0x4E8
    SkyMapLayer2 = 0x528
    SkyMapLayer3 = 0x568
    SkyMapLayer4 = 0x5A8
    SkyMapLayer5 = 0x5E8
    SkyMapLenV = 0x1FC
    SkyMapLenV1 = 0x50C
    SkyMapLenV2 = 0x54C
    SkyMapLenV3 = 0x58C
    SkyMapLenV4 = 0x5CC
    SkyMapLenV5 = 0x60C
    SkyMapURepeats1 = 0x504
    SkyMapURepeats2 = 0x544
    SkyMapURepeats3 = 0x584
    SkyMapURepeats4 = 0x5C4
    SkyMapURepeats5 = 0x604
    SkyMapUSpeed1 = 0x510
    SkyMapUSpeed2 = 0x550
    SkyMapUSpeed3 = 0x590
    SkyMapUSpeed4 = 0x5D0
    SkyMapUSpeed5 = 0x610
    SkySunColor = 0x3B4
    SkySunIntensity = 0x3C0
    SkyTextureBeginV1 = 0x4F8
    SkyTextureBeginV2 = 0x538
    SkyTextureBeginV3 = 0x578
    SkyTextureBeginV4 = 0x5B8
    SkyTextureBeginV5 = 0x5F8
    SkyTextureLenV1 = 0x4FC
    SkyTextureLenV2 = 0x53C
    SkyTextureLenV3 = 0x57C
    SkyTextureLenV4 = 0x5BC
    SkyTextureLenV5 = 0x5FC
    SkyfogBegin = 0x734
    SkyfogDensity = 0x73C
    SkyfogEnd = 0x738
    SnowIntensity = 0x244
    SnowPeriod = 0x240
    SnowRate = 0x228
    SpeedDown = 0x63C
    SpeedUp = 0x638
    StarCount = 0x454
    StarIntensity = 0x44C
    StarSize = 0x450
    SunAmbient = 0x324
    SunAmbientIntensity = 0x330
    SunBrightness = 0x3D8
    SunColor = 0x300
    SunColorIntensity = 0x30C
    SunDiskLumenScale = 0x7F4
    SunPerPixelAtmosphereTransmittance = 0x338
    SunSourceAngle = 0x334
    SunriseTime = 0x8EC
    SunsetTime = 0x8F0
    Temperature = 0x314
    TemperatureColor = 0x318
    TentTipAltitude = 0x7D0
    TentTipValue = 0x7D4
    TentWidth = 0x7D8
    ThunderIntensity = 0x264
    ThunderPeriod = 0x260
    ThunderRate = 0x248
    Time = 0x2F8
    TimeOfDay = 0x840
    TimeOfDayAutoPlay = 0x848
    TimeOfDayTrack = 0x858
    TimeOfDayUpdateThreshold = 0x84C
    TransmittanceMinLightElevationAngle = 0x7F0
    VignettingRate = 0x650
    VirtualLitColor = 0x1D4
    VirtualLitIntensity = 0x1E0
    VolumetricFogAlbedo = 0x744
    VolumetricFogDistance = 0x760
    VolumetricFogEmissive = 0x750
    VolumetricFogExtinctionScale = 0x75C
    VolumetricFogPhaseG = 0x740
    VolumetricFogZNear = 0x764
    WhiteTemp = 0x270
    WhiteTint = 0x274
    WindDir = 0x918
    WindStrength = 0x924
    Yaw = 0x350
    ZenithColor = 0x3F0
    ZenithIntensity = 0x3FC
    ZenithShift = 0x400

class BatchComponent:
    BatchKey = 0x35C
    InstanceBound = 0x360
    InstanceOccluIds = 0x300
    Instances = 0x2E8
    IsLightmapReady = 0x359
    LightmapAlwaysLoad = 0x358
    LightmapID = 0x318
    LightmapOffsets = 0x328
    LightmapScales = 0x340
    ReceiveDecals = 0x35A

class BezierCurveEdge:
    ArcParams = 0x28
    Length = 0x20
    RightControlPoint = 0x14

class BezierCurveMap:
    Edges = 0x18
    Length = 0x30

class BlastBondInfo:
    area = 0x14
    centroid = 0x18

class BlastChunkInfo:
    volume = 0x10

class BlendShapeAnim:
    Channels = 0x20
    Length = 0x18

class BlendShapeChannel:
    KeyValues = 0x18

class BlendShapeData:
    BlendShapeIndices = 0x20
    ChannelName = 0x50
    Deformation = 0x68
    RenderMappedIndices = 0x38

class BlendShapeTarget:
    DeltaPosition = 0x18

class BoardComponent:
    BoneName = 0xA8
    BoundingBox = 0x98
    ComplementalY = 0x78
    DepthEnable = 0x6A
    Displacement = 0x7C
    FovDistance = 0x70
    FrameEnable = 0x6B
    IsAbsoluteDisplacement = 0x6C
    IsRectConstrainted = 0x6D
    RectMargin = 0x88
    TopStick = 0x68
    Visibility = 0x69
    VisibleDistance = 0x74

class BondHealth:
    Area = 0x24
    NodeIndex0 = 0x1C
    NodeIndex1 = 0x20
    Normal = 0x10

class BonePlacer:
    BoneName = 0xA8
    Direction = 0xD8
    Finished = 0x9C
    Fov = 0xA0
    Height = 0x98
    PlacerPos = 0xCC
    Radius = 0x94
    Skeleton = 0x88
    Target = 0xE8
    TargetPos = 0xC0
    Yaw = 0x90

class BoxShapeData:
    Height = 0x14
    Length = 0xC
    Width = 0x10

class CNodeFloat:
    ArriveTangent = 0xC
    LeaveTangent = 0x10

class CNodeVector2:
    ArriveTangent = 0x10
    LeaveTangent = 0x18

class CNodeVector3:
    ArriveTangent = 0x14
    LeaveTangent = 0x20

class CNodeVector4:
    ArriveTangent = 0x18
    LeaveTangent = 0x28

class CSV parsing and writing:
    This module provides classes that assist in the reading and writing
of Comma Separated Value (CSV) files, and implements the interface
described by PEP 305.  Although many CSV files are simple to parse,
the format is not formally defined by a stable specification and
is subtle enough that parsing lines of a CSV file with something
like line.split(",") is bound to fail.  The module supports three
basic APIs: reading, writing, and registration of dialects.


DIALECT REGISTRATION:

Readers and writers support a dialect argument, which is a convenient
handle on a group of settings.  When the dialect argument is a string,
it identifies one of the dialects previously registered with the module.
If it is a class or instance, the attributes of the argument are used as
the settings for the reader or writer:

    class excel:
        delimiter = ','
        quotechar = '"'
        escapechar = None
        doublequote = True
        skipinitialspace = False
        lineterminator = '\r\n'
        quoting = QUOTE_MINIMAL

SETTINGS:

    * quotechar - specifies a one-character string to use as the
        quoting character.  It defaults to '"'.
    * delimiter - specifies a one-character string to use as the
        field separator.  It defaults to ','.
    * skipinitialspace - specifies how to interpret spaces which
        immediately follow a delimiter.  It defaults to False, which
        means that spaces immediately following a delimiter is part
        of the following field.
    * lineterminator -  specifies the character sequence which should
        terminate rows.
    * quoting - controls when quotes should be generated by the writer.
        It can take on any of the following module constants:

        csv.QUOTE_MINIMAL means only when required, for example, when a
            field contains either the quotechar or the delimiter
        csv.QUOTE_ALL means that quotes are always placed around fields.
        csv.QUOTE_NONNUMERIC means that quotes are always placed around
            fields which do not parse as integers or floating point
            numbers.
        csv.QUOTE_NONE means that quotes are never placed around fields.
    * escapechar - specifies a one-character string used to escape
        the delimiter when quoting is set to QUOTE_NONE.
    * doublequote - controls the handling of quotes inside fields.  When
        True, two consecutive quotes are interpreted as one during read,
        and when writing, each quote character embedded in the data is
        written as two quotes = 0x10

class CageBeam:
    D = 0x24
    IsSupportBeam = 0x28
    K = 0x20
    LongBound = 0x30
    MaxBrokenStress = 0x1C
    MaxCompressStress = 0x10
    MaxExpansionStress = 0x14
    MaxStress = 0xC
    PlasticCoef = 0x18
    ShortBound = 0x2C

class CageConfigData:
    Brakes = 0x34
    Differential = 0xC
    EngineInertia = 0x30
    GearRatios = 0x18
    Neutral = 0x14
    Reverse = 0x10
    TorqueCurve = 0x38

class CageFlexBodyInfo:
    GroupId = 0x58
    GroupName = 0x40
    LocalRotation = 0x24
    LocalTranslation = 0x18
    SourceModel = 0x30

class CageHydro:
    P0 = 0xC
    P1 = 0x10

class CageNode:
    CollisionGroup = 0x14
    Friction = 0x10
    Mass = 0xC

class CageNodeGroup:
    GroupId = 0x18
    Nodes = 0x30

class CageRawResource:
    Beams = 0x78
    CollisionMeshs = 0x108
    Contacters = 0xF0
    Coordinate = 0x50
    FlexBodies = 0xD8
    Hydros = 0x90
    NodeGroups = 0xC0
    Nodes = 0x60
    Wheels = 0xA8

class CageResource:
    Beams = 0x78
    CollisionMeshs = 0xF0
    Contacters = 0xD8
    Coordinate = 0x50
    FlexBodies = 0xC0
    Hydros = 0x90
    Nodes = 0x60
    Wheels = 0xA8

class CageScaler:
    TorqueScale = 0xC

class CageSystem:
    RawResource = 0x40
    Resource = 0x160

class CageWheel:
    ArmNode = 0x24
    AxisNode0 = 0x10
    AxisNode1 = 0x14
    Braked = 0x1C
    Friction = 0x58
    Mass = 0x28
    MaxBrokenStress = 0x54
    MaxCompressStress = 0x48
    MaxExpansionStress = 0x4C
    MaxStress = 0x44
    NearAttachNode = 0x38
    Nodes = 0x60
    NumRays = 0xC
    PlasticCoef = 0x50
    Propulsed = 0x20
    RimDamping = 0x40
    RimSpring = 0x3C
    SNode = 0x18
    Side = 0x34
    TireDamping = 0x30
    TireSpring = 0x2C

class CameraBlendTree:
    BlendRoot = 0x60

class CameraBlender:
    Next = 0x90
    Previous = 0x88

class CameraBlur:
    Enabled = 0x98
    FarTransitionRegion = 0xA0
    FocalRegion = 0x9C
    MaxBlurriness = 0x88
    MaxDirLen = 0x90
    MinBlurriness = 0x8C
    MinDirLen = 0x94

class CameraChecker:
    DirLenThreshold = 0x88

class CameraCollider:
    CollisionFilterInfo = 0xBC
    EnableCollider = 0xC8
    EnableOverlap = 0xC9
    EnableSmooth = 0xCB
    EnableSweepEvent = 0xB9
    EnableTargetCollider = 0xCA
    InvalidCollideInfo = 0xD8
    InvalidTargetCollideInfo = 0xD9
    MinDist = 0xC0
    OffsetWallSticking = 0xC4
    OverlapFilterInfo = 0xCC
    SmoothRatio = 0xD4
    SmoothThreshold = 0xD0
    SweepCollided = 0xB8
    SweepShape = 0x170
    TerrainFilterInfo = 0x178

class CameraController:
    AffiliatedCamera = 0x470
    ControlledCamera = 0x468
    Detector = 0x420
    EarlyTick = 0x431
    EnableAffiliatedFov = 0x4EA
    EnableKeyframeEvent = 0x4EC
    EnableRecord = 0x480
    LocateBlendTree = 0x450
    MaxRecordNum = 0x484
    PlacerCache = 0x438
    PlatformEntity = 0x428
    PostProcessorBlendTree = 0x460
    RecordedFov = 0x4D0
    RecordedTransform = 0x488
    SensorBlendTree = 0x458
    Suspend = 0x430
    SweepShape = 0x400
    TickRole = 0x418
    VirtualCtrlCamera = 0x478

class CameraEasing:
    AdditiveAffiliatedFovOffsetEasingEscaped = 0x114
    AdditiveAffiliatedFovOffsetEasingInfo = 0x154
    AdditiveFovOffsetEasingEscaped = 0x110
    AdditiveFovOffsetEasingInfo = 0x140
    AffiliatedFovEasingEscaped = 0x10C
    AffiliatedFovEasingInfo = 0x12C
    AnimTarget = 0x200
    BeginAffiliatedFov = 0x90
    BeginFov = 0x88
    BoneNameRotation = 0x1B8
    BonePositionOffset = 0x1DC
    BoneRotationOffset = 0x1E8
    CurrAdditiveAffiliatedFovOffset = 0x9C
    CurrAdditiveFovOffset = 0x98
    CurrAffiliatedFov = 0x94
    CurrFov = 0x8C
    FovEasingEscaped = 0x108
    FovEasingInfo = 0x118
    LastBoneRotation = 0x1D0
    RotateEasingEscaped = 0x188
    RotateEasingInfo = 0x18C
    UseAdditiveFovOffset = 0xA0
    UseBoneRotation = 0x1B0

class CameraFrame:
    Fov = 0x18
    InterpolateMode = 0x1C
    Pitch = 0xC
    Roll = 0x14
    Time = 0x20
    Yaw = 0x10

class CameraManager:
    AimPlacer.ApplyRuntimeInfo = 0x6D46072000
    AimPlacer.Rotate = 0x6D46072000
    AnimatedController.SetAnimation = 0x6D46072000
    CameraCollider.InitOverlapShape = 0x6D46072000
    CameraCollider.InitSweepShape = 0x6D46072000
    CameraController.ApplyRuntimeInfo = 0x6D46072000
    CameraController.BlendInPlacer = 0x6D46072000
    CameraController.GetClampedScreenPointFromWorldPoint = 0x6D46072000
    CameraController.GetCurrXzDirection = 0x6D46072000
    CameraController.GetLastRuntimeInfo = 0x6D46072000
    CameraController.GetRootLocatePlacer = 0x6D46072000
    CameraController.GetRuntimeInfo = 0x6D46072000
    CameraController.GetXzDirection = 0x6D46072000
    CameraController.PrintTree = 0x6D46072000
    CameraController.Release = 0x6D46072000
    CameraController.Retain = 0x6D46072000
    CameraController.SetPostProcessor = 0x6D46072000
    CameraController.SetSensor = 0x6D46072000
    CameraController.SetTickRole = 0x6D46072000
    CameraController.ToggleRecordEnabled = 0x6D46072000
    CameraEasing.ClearFovOffset = 0x6D46072000
    CameraEasing.GetEasingRuntimeInfo = 0x6D46072000
    CameraEasing.RotateEasing = 0x6D46072000
    CameraEasing.ZoomAdditiveFov = 0x6D46072000
    CameraEasing.ZoomFov = 0x6D46072000
    CameraMotor.Reset = 0x6D46072000
    CameraMotor.Stop = 0x6D46072000
    CameraMover.RotateFrom = 0x6D46072000
    CameraPlacer.PrintPlacer = 0x6D46072000
    CameraShaker.StartShake = 0x6D46072000
    DollyTrackController.GetCurveMap = 0x6D46072000
    DollyTrackController.SetCurveMap = 0x6D46072000
    EntityDetector.AddTarget = 0x6D46072000
    EntityDetector.ClearTargets = 0x6D46072000
    EntityDetector.DelTarget = 0x6D46072000
    EntityDetector.Enable = 0x6D46072000
    FollowPlacer.ApplyMover = 0x6D46072000
    FollowPlacer.ApplyRuntimeInfo = 0x6D46072000
    FollowPlacer.CheckFaceForward = 0x6D46072000
    FollowPlacer.PitchYaw = 0x6D46072000
    FollowPlacer.Reset = 0x6D46072000
    FollowPlacer.Rotate = 0x6D46072000
    FollowPlacer.SetupMover = 0x6D46072000
    FollowPlacer.Zoom = 0x6D46072000
    FpsMover.PitchYaw = 0x6D46072000
    FpsPlacer.AimAt = 0x6D46072000
    FpsPlacer.ApplyEasing = 0x6D46072000
    FpsPlacer.ApplyMover = 0x6D46072000
    FpsPlacer.ClearEasing = 0x6D46072000
    FpsPlacer.ClearMover = 0x6D46072000
    FpsPlacer.Rotate = 0x6D46072000
    FpsPlacer.SetupMover = 0x6D46072000
    FramePlacer.PushFrame = 0x6D46072000
    GroupFollowController.AddTarget = 0x6D46072000
    GroupFollowController.ClearTargets = 0x6D46072000
    GroupFollowController.DelTarget = 0x6D46072000
    ModifierBlender.TriggerBlend = 0x6D46072000
    PipeBlender.Freeze = 0x6D46072000
    PipeBlender.PushMotor = 0x6D46072000
    PipeBlender.Reset = 0x6D46072000
    PipeBlender.SetPrevPlacer = 0x6D46072000
    PipeBlender.Stop = 0x6D46072000
    PivotPlacer.ApplyRuntimeInfo = 0x6D46072000
    PivotPlacer.Rotate = 0x6D46072000
    PivotPlacer.SetupMover = 0x6D46072000
    SensorPipe.PushSensor = 0x6D46072000
    SlavedPlacer.AppendYawTrace = 0x6D46072000
    SlavedPlacer.ApplyMover = 0x6D46072000
    SlavedPlacer.ApplyRuntimeInfo = 0x6D46072000
    SlavedPlacer.ClearYawTrace = 0x6D46072000
    SlavedPlacer.PitchYaw = 0x6D46072000
    SlavedPlacer.Reset = 0x6D46072000
    SlavedPlacer.SetupMover = 0x6D46072000
    SlavedPlacer.Zoom = 0x6D46072000
    StackBlender.ApplyRuntimeInfo = 0x6D46072000
    SubviewportHelper.Clear = 0x6D46072000
    SubviewportHelper.GetCamera = 0x6D46072000
    SubviewportHelper.Setup = 0x6D46072000
    TpsCollider.InitSweepShape = 0x6D46072000
    TpsMover.PitchYaw = 0x6D46072000
    TpsPlacer.AimAt = 0x6D46072000
    TpsPlacer.ApplyEasing = 0x6D46072000
    TpsPlacer.ApplyMover = 0x6D46072000
    TpsPlacer.ApplyRuntimeInfo = 0x6D46072000
    TpsPlacer.CheckFaceForward = 0x6D46072000
    TpsPlacer.ClearEasing = 0x6D46072000
    TpsPlacer.ClearMover = 0x6D46072000
    TpsPlacer.IsAlreadyFirstRotate = 0x6D46072000
    TpsPlacer.PitchYaw = 0x6D46072000
    TpsPlacer.Reset = 0x6D46072000
    TpsPlacer.Rotate = 0x6D46072000
    TpsPlacer.SetupMover = 0x6D46072000
    TpsPlacer.Zoom = 0x6D46072000
    VirtualCameraController.AddController = 0x6D46072000
    VirtualCameraController.AfterUpdate = 0x6D46072000
    VirtualCameraController.BeforeUpdate = 0x6D46072000
    VirtualCameraController.DelController = 0x6D46072000
    VirtualCameraController.Update = 0x6D46072000
    YaFpsMover.PitchYaw = 0x6D46072000
    YaFpsPlacer.AimAt = 0x6D46072000
    YaFpsPlacer.ApplyMover = 0x6D46072000
    YaFpsPlacer.ClearMover = 0x6D46072000
    YaFpsPlacer.Rotate = 0x6D46072000
    YaFpsPlacer.SetupMover = 0x6D46072000
    YaPivotPlacer.ApplyRuntimeInfo = 0x6D46072000
    YaPivotPlacer.SetMajorRange = 0x6D46072000
    YawMover.UpdateClockwise = 0x6D46072000

class CameraModifier:
    Fov = 0x94
    Height = 0x9C
    HorizontalLength = 0x98
    TargetOffset = 0x88

class CameraMotor:
    BackToStart = 0xD4
    CurrMoveVector = 0xA0
    CurrRotateAngle = 0xC8
    EndFrame = 0xB4
    EscapedTime = 0xD8
    Finished = 0x90
    FrameCount = 0xB8
    InterpolateMode = 0x8C
    Invalid = 0x88
    MoveVector = 0x94
    MoveVectorLen = 0xB0
    MoveVectorType = 0xAC
    RotateAngle = 0xBC
    TotalTime = 0xDC

class CameraMover:
    BeginDirection = 0xC4
    BeginFov = 0x100
    BeginTargetPosOffset = 0xB8
    BoneName = 0x90
    CurrDirection = 0xF4
    CurrFov = 0x108
    CurrTargetPosOffset = 0xE8
    EndDirection = 0xDC
    EndFov = 0x104
    EndTargetPosOffset = 0xD0
    Finished = 0x10C
    MoveType = 0xA8
    Target = 0x120
    TargetPos = 0x110
    TargetTransform = 0x128
    UseBonePosition = 0x88

class CameraRuntimeInfo:
    affiliatedFov = 0x20
    controller = 0x28
    direction = 0xC
    fov = 0x1C
    roll = 0x18

class CameraShaker:
    Amplitude = 0x8C
    DecayCoeff = 0xA0
    Duration = 0x98
    Frequency = 0x94
    LoopCount = 0x9C
    Random = 0xB4
    RollAmplitude = 0x90
    ShakeDirection = 0xA8
    ShakeType = 0xA4

class CapsuleShapeData:
    Height = 0x10
    Radius = 0xC

class ChainMapping:
    EndDstToSrcTrans = 0x38

class CharCtrlComponent:
    AngularSpeed = 0x2E4
    AutoUpdate = 0x330
    CanRideOnCharCtrl = 0x2B4
    CharOverlapBufferInnerOffset = 0x2EC
    CharOverlapBufferOffset = 0x2F0
    ContactOffset = 0x2B0
    CtrlEntY = 0x2FF
    DisableSlide = 0x33C
    DynamicCDRadius = 0x30C
    Enable = 0x2FC
    EnableDynamicCollision = 0x308
    EnableFallDetecting = 0x2FE
    EnableRideVelocity = 0x6D46046000
    EnableShapeUpFitting = 0x301
    EnableStepUpSearch = 0x33D
    EnableStickToGround = 0x300
    ForcePassiveMode = 0x328
    Gravity = 0x278
    GravityScale = 0x3C8
    InterpolatePose = 0x332
    IsInTeleporting = 0x3A5
    IsSupported = 0x3DC
    LastRealVel = 0x2D4
    LinearVel = 0x2B8
    MaxAllowedDiff = 0x32C
    MaxAngularSpeed = 0x2E8
    MaxDroppingVel = 0x304
    MaxMotionAdvance = 0x3D8
    MaxPitch = 0x314
    MaxRoll = 0x318
    MaxShapeUpAng = 0x2A8
    MaxSlope = 0x2A4
    MotionScale = 0x2F4
    Navigator = 0x3E0
    NeedPosReady = 0x331
    NewCollisionResponse = 0x333
    NewCollisionResponseFactor = 0x334
    Pos = 0x284
    PushStrength = 0x290
    PushUpRatio = 0x294
    RealVel = 0x2C4
    RideVelocity = 0x6D46046000
    ShapeUpFittingByGroundNormalApproximate = 0x310
    SimLevel = 0x2FD
    SlideCDTime = 0x338
    StepOffset = 0x2AC
    SupportedDelta = 0x2F8
    Up = 0x298
    Yaw = 0x2E0

class CharCtrlComponentNew:
    ExtraLinearVel = 0x5B0
    MaxSpeed = 0x5BC
    TickOnEngine = 0x5C2
    UpRot = 0x5A0
    UseNewSlideSearch = 0x5C0

class CharacterImp:
    ActorComponent.ActorParent = 0x6D46072000
    ActorComponent.AddChild = 0x6D46072000
    ActorComponent.AddDependency = 0x6D46072000
    ActorComponent.AddTarget = 0x6D46072000
    ActorComponent.AddTargetSlot = 0x6D46072000
    ActorComponent.ApplyGraphSyncToChild = 0x6D46072000
    ActorComponent.ApplyGraphSyncToHorse = 0x6D46072000
    ActorComponent.AttachRigidBody = 0x6D46072000
    ActorComponent.AttachSound = 0x6D46072000
    ActorComponent.Children = 0x6D46072000
    ActorComponent.ClearChildren = 0x6D46072000
    ActorComponent.ClearEffectFromGraph = 0x6D46072000
    ActorComponent.ClearEffectFromGraphImmediately = 0x6D46072000
    ActorComponent.ClearEventQueue = 0x6D46072000
    ActorComponent.ClearGraphCache = 0x6D46072000
    ActorComponent.ClearGraphStack = 0x6D46072000
    ActorComponent.ClearSignalQueue = 0x6D46072000
    ActorComponent.ClearTargets = 0x6D46072000
    ActorComponent.CreateFullPose = 0x6D46072000
    ActorComponent.DebugOutput = 0x6D46072000
    ActorComponent.DelChild = 0x6D46072000
    ActorComponent.DelDependency = 0x6D46072000
    ActorComponent.DelTarget = 0x6D46072000
    ActorComponent.DelTargetSlot = 0x6D46072000
    ActorComponent.DetachRigidBody = 0x6D46072000
    ActorComponent.DisableSignalTypes = 0x6D46072000
    ActorComponent.EnableSignalTypes = 0x6D46072000
    ActorComponent.FindLocomotionMatcherClosestFrame = 0x6D46072000
    ActorComponent.FireEvent = 0x6D46072000
    ActorComponent.GetActionPlaybackSpeed = 0x6D46072000
    ActorComponent.GetActionState = 0x6D46072000
    ActorComponent.GetAllNodeRuntimeInfoDict = 0x6D46072000
    ActorComponent.GetAllTargetMarks = 0x6D46072000
    ActorComponent.GetChildShareEventGraphs = 0x6D46072000
    ActorComponent.GetConnectingMark = 0x6D46072000
    ActorComponent.GetCurrentSlope = 0x6D46072000
    ActorComponent.GetDependencies = 0x6D46072000
    ActorComponent.GetDisabledSignalTypes = 0x6D46072000
    ActorComponent.GetDockingState = 0x6D46072000
    ActorComponent.GetEnabledSignalTypes = 0x6D46072000
    ActorComponent.GetFrameLimit = 0x6D46072000
    ActorComponent.GetFrameRecordNodes = 0x6D46072000
    ActorComponent.GetFrameRecordNodesEx = 0x6D46072000
    ActorComponent.GetGlobalFrameRecordNodesEx = 0x6D46072000
    ActorComponent.GetGraphActionPlaybackSpeed = 0x6D46072000
    ActorComponent.GetGraphConfig = 0x6D46072000
    ActorComponent.GetGraphResource = 0x6D46072000
    ActorComponent.GetGraphStack = 0x6D46072000
    ActorComponent.GetGraphStackInfo = 0x6D46072000
    ActorComponent.GetGraphSyncChild = 0x6D46072000
    ActorComponent.GetGraphSyncMode = 0x6D46072000
    ActorComponent.GetHoldingMark = 0x6D46072000
    ActorComponent.GetInvisibleFrameLimit = 0x6D46072000
    ActorComponent.GetMotionCaptureFrameRate = 0x6D46072000
    ActorComponent.GetMotionCaptureMaxFrameCount = 0x6D46072000
    ActorComponent.GetMotionMask = 0x6D46072000
    ActorComponent.GetNodeOutput = 0x6D46072000
    ActorComponent.GetNodeOutputEx = 0x6D46072000
    ActorComponent.GetNodePlaybackTime = 0x6D46072000
    ActorComponent.GetPoseBoneParentIds = 0x6D46072000
    ActorComponent.GetPoseModifier = 0x6D46072000
    ActorComponent.GetRagdollRigidBodyNames = 0x6D46072000
    ActorComponent.GetRagdollRigidBodyParents = 0x6D46072000
    ActorComponent.GetRigidBodyAttachments = 0x6D46072000
    ActorComponent.GetSamplePoseLod = 0x6D46072000
    ActorComponent.GetState = 0x6D46072000
    ActorComponent.GetTargetMark = 0x6D46072000
    ActorComponent.GetTargetPoint = 0x6D46072000
    ActorComponent.GetVariableF = 0x6D46072000
    ActorComponent.GetVariableI = 0x6D46072000
    ActorComponent.GetVariableM = 0x6D46072000
    ActorComponent.GetVariableP = 0x6D46072000
    ActorComponent.GetVariableS = 0x6D46072000
    ActorComponent.GetVariableV3 = 0x6D46072000
    ActorComponent.GetVariableZ = 0x6D46072000
    ActorComponent.Horse = 0x6D46072000
    ActorComponent.Identifier = 0x6D46072000
    ActorComponent.InsertGraph = 0x6D46072000
    ActorComponent.IsAutoPlayEffect = 0x6D46072000
    ActorComponent.IsChildShareEvents = 0x6D46072000
    ActorComponent.IsChildTickBeforeParent = 0x6D46072000
    ActorComponent.IsEnableBottomGraphSync = 0x6D46072000
    ActorComponent.IsEnableControlCamera = 0x6D46072000
    ActorComponent.IsEnableControlLight = 0x6D46072000
    ActorComponent.IsEnableInertializationForSlave = 0x6D46072000
    ActorComponent.IsEnableInternalClimbingIKSolver = 0x6D46072000
    ActorComponent.IsEnableMotionCapture = 0x6D46072000
    ActorComponent.IsEnableMotionScale = 0x6D46072000
    ActorComponent.IsEnablePlayGraphEffect = 0x6D46072000
    ActorComponent.IsEnablePlayGraphSound = 0x6D46072000
    ActorComponent.IsEnableVisibilityOptimize = 0x6D46072000
    ActorComponent.IsGraphSleeping = 0x6D46072000
    ActorComponent.JumpToState = 0x6D46072000
    ActorComponent.LoadSkeletonAndGraph = 0x6D46072000
    ActorComponent.LoadSkeletonAndGraphDeferred = 0x6D46072000
    ActorComponent.MotionPlaybackInput = 0x6D46072000
    ActorComponent.PauseGraph = 0x6D46072000
    ActorComponent.PopGraph = 0x6D46072000
    ActorComponent.PopGraphByID = 0x6D46072000
    ActorComponent.PopGraphByName = 0x6D46072000
    ActorComponent.PoseStreamInput = 0x6D46072000
    ActorComponent.PrintGraphSyncState = 0x6D46072000
    ActorComponent.PrintSlaveNodes = 0x6D46072000
    ActorComponent.PushGraph = 0x6D46072000
    ActorComponent.PushGraphFromMem = 0x6D46072000
    ActorComponent.QueryCues = 0x6D46072000
    ActorComponent.QueryEvents = 0x6D46072000
    ActorComponent.QueryFirstNodeByName = 0x6D46072000
    ActorComponent.QueryLocomotionMatcherRecords = 0x6D46072000
    ActorComponent.QueryMotionCaptureResults = 0x6D46072000
    ActorComponent.QueryNodeExist = 0x6D46072000
    ActorComponent.QueryNodeIDByNodePath = 0x6D46072000
    ActorComponent.QueryNodePathByNodeID = 0x6D46072000
    ActorComponent.QueryNodesByName = 0x6D46072000
    ActorComponent.QueryTimelineNodeInfo = 0x6D46072000
    ActorComponent.RayCastCollisionBone = 0x6D46072000
    ActorComponent.ReloadAll = 0x6D46072000
    ActorComponent.ReloadGraph = 0x6D46072000
    ActorComponent.ReloadGraphFromMem = 0x6D46072000
    ActorComponent.ReloadSkeleton = 0x6D46072000
    ActorComponent.RemoveActionCueType = 0x6D46072000
    ActorComponent.ReplaceGraph = 0x6D46072000
    ActorComponent.ResetAllGraph = 0x6D46072000
    ActorComponent.ResetGraph = 0x6D46072000
    ActorComponent.ResetRagdollPose = 0x6D46072000
    ActorComponent.ResetTPose = 0x6D46072000
    ActorComponent.Ride = 0x6D46072000
    ActorComponent.SetActionPlaybackSpeed = 0x6D46072000
    ActorComponent.SetActionPlaybackSpeedTransient = 0x6D46072000
    ActorComponent.SetAllGraphSleeping = 0x6D46072000
    ActorComponent.SetAutoFrameLimit = 0x6D46072000
    ActorComponent.SetAutoPlayEffect = 0x6D46072000
    ActorComponent.SetChildShareEventGraphs = 0x6D46072000
    ActorComponent.SetChildShareEvents = 0x6D46072000
    ActorComponent.SetChildTickBeforeParent = 0x6D46072000
    ActorComponent.SetDebugDrawMask = 0x6D46072000
    ActorComponent.SetEditingMode = 0x6D46072000
    ActorComponent.SetEnableBottomGraphSync = 0x6D46072000
    ActorComponent.SetEnableControlCamera = 0x6D46072000
    ActorComponent.SetEnableControlLight = 0x6D46072000
    ActorComponent.SetEnableInertializationForSlave = 0x6D46072000
    ActorComponent.SetEnableInternalClimbingIKSolver = 0x6D46072000
    ActorComponent.SetEnableMotionCapture = 0x6D46072000
    ActorComponent.SetEnableMotionScale = 0x6D46072000
    ActorComponent.SetEnablePlayGraphEffect = 0x6D46072000
    ActorComponent.SetEnablePlayGraphSound = 0x6D46072000
    ActorComponent.SetEnableVisibilityOptimize = 0x6D46072000
    ActorComponent.SetFrameLimit = 0x6D46072000
    ActorComponent.SetGraphActionPlaybackSpeed = 0x6D46072000
    ActorComponent.SetGraphActionPlaybackSpeedTransient = 0x6D46072000
    ActorComponent.SetGraphBlendTime = 0x6D46072000
    ActorComponent.SetGraphConfig = 0x6D46072000
    ActorComponent.SetGraphSleeping = 0x6D46072000
    ActorComponent.SetGraphSyncActorVariables = 0x6D46072000
    ActorComponent.SetGraphSyncMaster = 0x6D46072000
    ActorComponent.SetGraphSyncSlave = 0x6D46072000
    ActorComponent.SetInvisibleFrameLimit = 0x6D46072000
    ActorComponent.SetMotionCaptureFrameRate = 0x6D46072000
    ActorComponent.SetMotionCaptureMaxFrameCount = 0x6D46072000
    ActorComponent.SetMotionMask = 0x6D46072000
    ActorComponent.SetOthersGraphSleeping = 0x6D46072000
    ActorComponent.SetPoseModifier = 0x6D46072000
    ActorComponent.SetSamplePoseLod = 0x6D46072000
    ActorComponent.SetSendActivatedSignal = 0x6D46072000
    ActorComponent.SetSendActivatedSignalByNodeID = 0x6D46072000
    ActorComponent.SetTargetPoint = 0x6D46072000
    ActorComponent.SetTargetPointToBone = 0x6D46072000
    ActorComponent.SetTimeLineTransform = 0x6D46072000
    ActorComponent.SetVariableF = 0x6D46072000
    ActorComponent.SetVariableI = 0x6D46072000
    ActorComponent.SetVariableM = 0x6D46072000
    ActorComponent.SetVariableP = 0x6D46072000
    ActorComponent.SetVariableS = 0x6D46072000
    ActorComponent.SetVariableV3 = 0x6D46072000
    ActorComponent.SetVariableZ = 0x6D46072000
    ActorComponent.SetVariablesByTable = 0x6D46072000
    ActorComponent.ShiftGraph = 0x6D46072000
    ActorComponent.ShowRagdollRigidBodies = 0x6D46072000
    ActorComponent.SphereSweepCollisionBone = 0x6D46072000
    ActorComponent.TargetSlots = 0x6D46072000
    ActorComponent.Targets = 0x6D46072000
    ActorComponent.TriggerSignal = 0x6D46072000
    ActorComponent.UpdateMotionProviderData = 0x6D46072000
    ActorComponent.UpdateSkeletonDeformerNodeData = 0x6D46072000
    ActorComponent.UpdateTimelineTrack = 0x6D46072000
    DockingMarkComponent.GetMarks = 0x6D46072000
    FilterComponent.GetJumpState = 0x6D46072000
    FilterComponent.GetMotionMask = 0x6D46072000
    FilterComponent.GetMoveState = 0x6D46072000
    FilterComponent.GetPlatformEntity = 0x6D46072000
    FilterComponent.GetPoseInputCallback = 0x6D46072000
    FilterComponent.InputPose = 0x6D46072000
    FilterComponent.InputPoseExt = 0x6D46072000
    FilterComponent.InputPoseExt2 = 0x6D46072000
    FilterComponent.InputPoseRaw = 0x6D46072000
    FilterComponent.QueryReceivePackRecords = 0x6D46072000
    FilterComponent.Reset = 0x6D46072000
    FilterComponent.ResetExt = 0x6D46072000
    FilterComponent.ResetExt2 = 0x6D46072000
    FilterComponent.SetFilterType = 0x6D46072000
    FilterComponent.SetMotionMask = 0x6D46072000
    FilterComponent.SetMotionMaskExt = 0x6D46072000
    FilterComponent.SetPlatformEntity = 0x6D46072000
    LocalPoseModifier.DumpBinaryData = 0x6D46072000
    LocalPoseModifier.GetAllBones = 0x6D46072000
    LocalPoseModifier.GetBoneModifier = 0x6D46072000
    LocalPoseModifier.RemoveAll = 0x6D46072000
    LocalPoseModifier.RemoveBoneModifier = 0x6D46072000
    LocalPoseModifier.SetBinaryData = 0x6D46072000
    LocalPoseModifier.SetBoneModifier = 0x6D46072000
    PoseSenderComponent.QuerySendPackRecords = 0x6D46072000
    PoseSenderComponent.SetJumpState = 0x6D46072000
    PoseSenderComponent.SetPlatformEntity = 0x6D46072000
    PoseSenderComponent.SetPoseReceiver = 0x6D46072000
    SceneGuiderComponent.OnRigidBodiesChanging = 0x6D46072000

class ClientManipulator:
    ActiveGizmo = 0x2B0
    Camera = 0x220
    GizmoSize = 0x2B8
    Gizmos = 0x288
    SubviewManager = 0x280
    Target = 0x228
    TargetTransform = 0x250
    World = 0x218

class ClientNavigator:
    Camera = 0x1F8
    CameraController = 0x200
    MoveSpeed = 0x208

class ClientPlayer:
    AffiliatedCameras = 0x230
    Camera = 0x228
    InterceptInputByFilter = 0x278
    Manipulator = 0x268
    Navigator = 0x270
    PrincipleRole = 0x260

class ClientScenario:
    ActiveWorld = 0x200
    AsyncLoadWorld = 0x241
    DelayDeserialization = 0x240
    DeserializationTimeSlice = 0x238
    LinkedWorlds = 0x210
    Sound = 0x208

class ClothAirMeshData:
    ClothAirVolume = 0x18

class ClothAirWallSkinData:
    BoneIndices = 0x18
    Radius = 0x30
    WallRefDirection = 0x60
    WallRefPosition = 0x48

class ClothCollisionCache:
    AirMeshData = 0x78

class ClothCouplingRigidBody:
    AngleVelocity = 0x3C
    LinearVelocity = 0x30
    Mass = 0x48

class ClothMeshAnchorTable:
    BoneRefMatrix = 0x30
    BoneRefTriIndex = 0x18

class ClothMeshConfigData:
    AirDensityScale = 0x68
    AirDragCoefficient = 0x60
    AirLiftCoefficient = 0x64
    AirMeshVolumeScale = 0xE4
    AngularDrag = 0xA8
    AngularInertia = 0xC0
    AnimDriveDamping = 0x74
    AnimDriveMultiplier = 0x70
    CentrifugalInertia = 0xCC
    ClothWindMethodType = 0x7C
    CollisionMassScale = 0x8C
    CollisionThickness = 0xE8
    CompressionLimit = 0x38
    CpuTangentUpdate = 0x91
    Damping = 0x54
    EnableCCD = 0x90
    Friction = 0x88
    GravityScale = 0x80
    InvMassToArea = 0x78
    LinearDrag = 0x9C
    LinearInertia = 0xB4
    MassScale = 0x84
    SelfCollisionCullScale = 0x50
    SelfCollisionStiffness = 0x4C
    SelfCollisionThickness = 0x48
    ShapeMatchingFactor = 0xE0
    SolverFreq = 0x6C
    StiffnessFreq = 0x98
    StiffnessMultiplier = 0x18
    StretchLimit = 0x28
    TetherLimit = 0xDC
    TetherStiffness = 0xD8
    VirtualParticleLevel = 0x94

class ClothMeshConstrainData:
    AirMeshMask = 0x30
    CollisionCache = 0x18

class ClothMeshMergeTable:
    ClothIndices = 0x28
    VertexMapper = 0x10

class ClothMeshStaticData:
    AnchorTable = 0xD8
    ClothBoneCounts = 0x2D0
    ClothBoneIndices = 0x2A0
    ClothBoneWeights = 0x288
    ClothCollisionCache = 0x120
    ClothIndices = 0x258
    ClothInvMass = 0x270
    ClothPositions = 0x210
    ClothTPosePositions = 0x240
    ClothTable = 0x80
    ClothUvs = 0x228
    Configs = 0x20
    ConstrainData = 0x38
    RenderIndices = 0x1E0
    RenderPositions = 0x1C8
    RenderUvs = 0x1F8
    WrapTable = 0xC0

class ClothSimulationGroup:
    SelfCollisionIndices = 0x18
    StartVertex = 0xC
    VerticesCount = 0x10

class ClothSimulatorComponent:
    ColShapeAssetPath = 0xF8
    CollisionFilterInfo = 0x14C
    CollisionModifier = 0x130
    ControlBoneName = 0x170
    ControlledByMutiBone = 0x16A
    CouplingConfig = 0x1F0
    CouplingRigidBodyInOut = 0x208
    EnableInterClothAirmesh = 0x14A
    EnablePhysicsSpaceCollision = 0x14B
    EnableSelfAirmesh = 0x149
    Gravity = 0x110
    KeepAnimShape = 0x12C
    LeftShoulder = 0x188
    LinearSpeedSensitivity = 0x128
    LowFpsProtection = 0x158
    MaxBoundMovement = 0x160
    MaxDeltaRotation = 0x154
    MaxDeltaTranslation = 0x150
    MaxDisScale = 0x15C
    RemoveMotion = 0x148
    ResetOnce = 0x169
    RightShoulder = 0x1A0
    SolverFreqOverride = 0x1BC
    WaitAnimated = 0x1B8
    Wind = 0x11C

class CmParticleElementSource:
    ActiveDuration = 0x68
    EmissionBurst = 0x60
    EmissionDelay = 0x5C
    EmissionDuration = 0x58
    EmissionRate = 0x50
    EmissionRateByDistance = 0x54
    SleepDuration = 0x6C
    TimeTriggered = 0x64

class CollisionFilterData:
    Data3 = 0xC

class CollisionGroupColorItem:
    a = 0x10
    b = 0xC

class CollisionResult:
    AuxData = 0x58
    Body = 0x38
    Distance = 0x4C
    Flags = 0x50
    IsHit = 0x48
    MaterialTypeId = 0x54
    Normal = 0x28
    Shape = 0x40

class CommonVehicleController:
    EnableAutoReverse = 0x49
    GearDown = 0x46
    GearUp = 0x45
    KeyPressedAccel = 0x40
    KeyPressedBrake = 0x41
    KeyPressedHandbrake = 0x42
    KeyPressedSteerLeft = 0x43
    KeyPressedSteerRight = 0x44
    PadAccel = 0x4C
    PadBrake = 0x50
    PadHandbrake = 0x54
    PadSteer = 0x58
    UseAutoGears = 0x48
    UseKey = 0x47

class ConstraintComponent:
    ConstraintParams = 0xD0

class ConstraintConfig:
    BreakForceThreshold = 0x6D46046000
    BreakTorqueThreshold = 0x6D46046000
    ConstraintType = 0x6D46046000
    EnableProjection = 0x6D46046000
    IsBreakable = 0x6D46046000
    ProjectionAngularTolerance = 0x6D46046000
    ProjectionLinearTolerance = 0x6D46046000
    RevoluteConfig = 0x50
    Trans0 = 0x6D46046000
    Trans1 = 0x6D46046000

class ConstraintDataBase:
    Actor0Id = 0x10
    Actor1Id = 0x28
    BreakForceThreshold = 0xAC
    BreakTorqueThreshold = 0xB0
    EnableProjection = 0xA0
    IsBreakable = 0xB4
    ProjectionAngularTolerance = 0xA8
    ProjectionLinearTolerance = 0xA4
    Trans0 = 0x40
    Trans1 = 0x70

class ConstraintRevoluteConfig:
    DriveSpeed = 0x6D46046000
    EnableDrive = 0x6D46046000
    EnableRotLimit = 0x6D46046000
    RotLimitLower = 0x6D46046000
    RotLimitUpper = 0x6D46046000

class ContactInfo:
    AbsPos = 0x58
    Flag = 0x48
    Impulse = 0x7C
    MaterialTypeIdA = 0x40
    MaterialTypeIdB = 0x44
    Normal = 0x70
    Pos = 0x4C
    ShapeA = 0x30
    ShapeB = 0x38

class Control the number of TLSv1:
    3 session tickets = 0x50

class ConvexMeshShapeData:
    Vertices = 0x10

class CookedCurve:
    CurveGuid = 0x10

class CoupleFocusLocator:
    Direction = 0xB8
    MajorTarget = 0xC8
    MajorTargetPos = 0x88
    MajorTargetPosOffset = 0xA0
    MajorTargetTransform = 0xD8
    MinorTarget = 0xD0
    MinorTargetPos = 0x94
    MinorTargetPosOffset = 0xAC
    MinorTargetTransform = 0x108

class Create a cached callable that wraps another function:
    user_function:      the function being cached

maxsize:  0         for no caching
          None      for unlimited cache size
          n         for a bounded cache

typed:    False     cache f(3) and f(3.0) as identical calls
          True      cache f(3) and f(3.0) as distinct calls

cache_info_type:    namedtuple class with the fields:
                        hits misses currsize maxsize = 0x10

class CubeLocalEnvVolumeComponent:
    Enable = 0x9DD
    FadeDistance = 0x9E0

class CuePoint:
    insure = 0x21
    oneshot = 0x20

class CurveInfo:
    Dimension = 0x1A
    MaxTime = 0x14
    MinTime = 0x10
    Row = 0x18

class DamageParams:
    ChunkIndex0 = 0x84
    ChunkIndex1 = 0x88
    Damage = 0x74
    DamageType = 0x40
    ForceBreak = 0xA8
    Groups = 0x90
    MaxRadius = 0x7C
    MinRadius = 0x78
    Normal = 0x68
    Point0 = 0x44
    Point1 = 0x50
    Point2 = 0x5C
    Radius = 0x80

class DecalComponent:
    Birthday = 0x14C
    FadeTime = 0x148
    MaxLifeTime = 0x144
    OccluId = 0x120
    Resource = 0xF8
    TintColor = 0x134
    TintIntensity = 0x140
    UBegin = 0x124
    UEnd = 0x12C
    VBegin = 0x128
    VEnd = 0x130
    WorldBound = 0x108

class DestructibleAssetData:
    CollisionProperties = 0x28
    DynamicProperties = 0x4C
    PhysicsMaterial = 0x18

class DestructibleDebrisFilterData:
    DebrisLifetimeMax = 0x30
    DebrisLifetimeMin = 0x2C
    DebrisMaxSize = 0xC
    DebrisNoCollisionLiftime = 0x34
    UseValidBounds = 0x10
    ValidBounds = 0x14

class DestructibleExtraData:
    DecorateData = 0x18

class DestructibleExtraDecorateData:
    LocalPos = 0x18
    Rotation = 0x24
    Scale = 0x34

class DestructibleExtraPhyMaterialData:
    DynamicProperties = 0x18

class DestructibleFamily:
    Actors = 0x1A0
    AssetPath = 0xD0
    DMesh = 0x198
    DispDecos = 0xE9
    Level = 0x1B8
    Pause = 0x1E8
    Reset = 0xE8

class DestructibleModelComponent:
    DModelPath = 0x288
    IsLightmapConst = 0x390
    IsLightmapReady = 0x391
    Lightmap = 0x2A0
    LightmapOffset = 0x380
    LightmapScale = 0x370

class DiheralBendCons:
    ResetAngle = 0x10

class DistributionConstantFloat:
    Value = 0x40

class DistributionConstantVector2:
    Value = 0x40

class DistributionConstantVector3:
    Value = 0x40

class DistributionConstantVector4:
    Value = 0x40

class DistributionCurveFloat:
    Curve = 0x40
    Discrete = 0x58

class DistributionCurveVector2:
    Curve = 0x40
    Discrete = 0x58

class DistributionCurveVector3:
    Curve = 0x40
    Discrete = 0x58

class DistributionCurveVector4:
    Curve = 0x40
    Discrete = 0x58

class DistributionCylinder:
    Axis = 0x4C
    Center = 0x40
    MaxRadius = 0x5C
    MinRadius = 0x58

class DistributionSphere:
    Center = 0x40
    MaxRadius = 0x50
    MinRadius = 0x4C

class DistributionUniformFloat:
    MaxValue = 0x44
    MinValue = 0x40

class DistributionUniformVector2:
    MaxValue = 0x48
    MinValue = 0x40

class DistributionUniformVector3:
    MaxValue = 0x4C
    MinValue = 0x40

class DistributionUniformVector4:
    MaxValue = 0x50
    MinValue = 0x40

class DockingMarkComponent:
    Division = 0x90
    FilePath = 0x78
    IsAnimated = 0xC0
    IsLoading = 0xC1
    OffsetTransforms = 0xA8

class DollyTrackController:
    LerpRate = 0x11C
    Percent = 0x114
    Position = 0x124
    Subdiv = 0x118
    Target = 0x100
    Tolerance = 0x120
    Transform = 0x130

class DoubleCurveObjectFloat:
    FirstCurve = 0x20
    LerpFactor = 0x18
    Mode = 0x14
    SecondCurve = 0x38

class DoubleCurveObjectVector2:
    FirstCurve = 0x20
    LerpFactor = 0x18
    Mode = 0x14
    SecondCurve = 0x38

class DoubleCurveObjectVector3:
    FirstCurve = 0x20
    LerpFactor = 0x18
    Mode = 0x14
    SecondCurve = 0x38

class DoubleCurveObjectVector4:
    FirstCurve = 0x20
    LerpFactor = 0x18
    Mode = 0x14
    SecondCurve = 0x38

class ERenderTargetResourceInfo:
    ClearColor = 0xC
    IsPersist = 0x1C
    RenderTargetType = 0x1D

class EasingRuntimeInfo:
    affiliatedFovEasingFinished = 0x34
    affiliatedFovOffsetEasingFinished = 0x35
    currAdditiveAffiliatedFovOffset = 0xC
    currBonePositionOffset = 0x10
    currBoneRotationOffset = 0x1C
    currPitchYawDelta = 0x28
    fovEasingFinished = 0x32
    fovOffsetEasingFinished = 0x33
    rotateEasingFinished = 0x36
    useAdditiveFovOffset = 0x31
    useBoneRotation = 0x30

class EntityDetector:
    Center = 0x3D8
    IsEnable = 0x3D0
    Radius = 0x3D4

class EventPoint:
    time = 0x18

class ExceptHookArgs

Type used to pass arguments to threading:
    excepthook. = 0x10

class ExtImageIO:
    AnimatedWebP.Cache = 0x6D46072000
    AnimatedWebP.Flush = 0x6D46072000
    AnimatedWebP.LoadAnimatedWebP = 0x6D46072000

class FLMSphere:
    Offset = 0x20

class FilterComponent:
    ApplyMotion = 0x80
    CollisionFilterInfo = 0x88
    DebugDraw = 0x83
    DebugOutput = 0x81
    DebugOutput2 = 0x82
    DriveRigidBody = 0x94
    EnableDrop = 0x84
    EnableOptimize = 0x8C
    ForceDropTest = 0x8E
    InputInterval = 0x90
    OptimizeDelay = 0x8D
    PlatformEntity = 0x98
    UseHeightMap = 0x85

class Flamenco:
    FlamencoCageRuntimeParameter.ApplyImpulse = 0x6D46072000
    FlamencoComponent.GetSkinSkeletonProvider = 0x6D46072000

class FlamencoCageRuntimeParameter:
    Accel = 0x40
    Brake = 0x41
    Reverse = 0x42
    SteerLeft = 0x43
    SteerRight = 0x44
    SystemReporter = 0x58
    SystemScaler = 0x48

class FlamencoCageSystemData:
    CageSystem = 0x40

class FlamencoComponent:
    Parameter = 0x288

class FlamencoGarmentData:
    RawResource = 0x40
    Resource = 0x198

class FlamencoGarmentDataCollection:
    GarmentData = 0x40

class FlamencoHairDataCollection:
    GeometryData = 0x48
    ParameterData = 0x40

class FlamencoHairGeometryData:
    InterpolationData = 0x58
    RenderStraints = 0x40

class FlamencoHairInterpolationData:
    DriveCurveIndices = 0x58
    DriveGlobalIndices = 0x70
    GuideCurveIndices = 0x40
    GuideVertexWeights = 0xA0
    TangentDirectionLerps = 0x88

class FlamencoHairParameter:
    AnchorBoneName = 0x88
    GlobalDamping = 0xD4
    GlobalShapeMatchingFactor = 0xE0
    GravityScale = 0xD0
    GuideHairCount = 0x48
    HairLengthScale = 0x68
    IterationCount = 0xDC
    LocalBound = 0x6C
    LodDistance = 0x4C
    LodFator = 0x50
    NormalHairCount = 0x44
    RefMatrix = 0xA0
    Scale = 0x40
    Spheres = 0x100
    TranslationScale = 0xD8
    TwistStiffnessFactor = 0xF8
    VolumeCollisionForce = 0xFC

class FlamencoHairRuntimeParameter:
    HideHair = 0x40
    WindDirection = 0x44
    WindFrequency = 0x54
    WindSpeed = 0x50

class FlareProperty:
    Distance = 0xC
    FlareColor = 0x10
    FlareIntensity = 0x1C
    SizeInPixels = 0x20
    SizeInPixelsH = 0x24
    flareIndex = 0x28

class FluidDebugDrawer:
    Show = 0x60
    VelMagRange = 0x64
    ViewPortPos = 0x6C

class FluidSim:
    ShallowWaterComponent.GetLodLevel = 0x6D46072000
    ShallowWaterComponent.GetLodLevelLocked = 0x6D46072000
    ShallowWaterComponent.GetShowTerrain = 0x6D46072000
    ShallowWaterComponent.GetShowVelocity = 0x6D46072000
    ShallowWaterComponent.GetShowWireframe = 0x6D46072000
    ShallowWaterComponent.GetSimulatePause = 0x6D46072000
    ShallowWaterComponent.LoadWaterSurface = 0x6D46072000
    ShallowWaterComponent.LockAndSetLodLevel = 0x6D46072000
    ShallowWaterComponent.LockLodLevel = 0x6D46072000
    ShallowWaterComponent.SaveWaterSurface = 0x6D46072000
    ShallowWaterComponent.SetEnableRenderer = 0x6D46072000
    ShallowWaterComponent.SetInteractionParams = 0x6D46072000
    ShallowWaterComponent.SetShowTerrain = 0x6D46072000
    ShallowWaterComponent.SetShowVelocity = 0x6D46072000
    ShallowWaterComponent.SetShowWireframe = 0x6D46072000
    ShallowWaterComponent.SetSimulatePause = 0x6D46072000
    ShallowWaterComponent.SetWaterWaveResource = 0x6D46072000
    ShallowWaterInteractorComponent.AddAttachedShape = 0x6D46072000
    ShallowWaterInteractorComponent.AddSphereInteractor = 0x6D46072000
    ShallowWaterInteractorComponent.RemoveAttachedShapes = 0x6D46072000
    ShallowWaterInteractorComponent.SetDrawInteractors = 0x6D46072000
    ShallowWaterInteractorComponent.SetInteractorType = 0x6D46072000
    ShallowWaterInteractorComponent.SetShallowWaterComponent = 0x6D46072000
    ShallowWaterSourceComponent.GetHeightMode = 0x6D46072000
    ShallowWaterSourceComponent.GetShapeSize = 0x6D46072000
    ShallowWaterSourceComponent.GetSourceHeight = 0x6D46072000
    ShallowWaterSourceComponent.GetSourceVelocity = 0x6D46072000
    ShallowWaterSourceComponent.IsShapeCircle = 0x6D46072000
    ShallowWaterSourceComponent.IsVelcoityUsed = 0x6D46072000
    ShallowWaterSourceComponent.SetAsCircleShape = 0x6D46072000
    ShallowWaterSourceComponent.SetAsRectangleShape = 0x6D46072000
    ShallowWaterSourceComponent.SetShallowWaterComponent = 0x6D46072000
    ShallowWaterSourceComponent.SetSourceInfo = 0x6D46072000
    ShallowWaterSourceComponent.SetSourceShape = 0x6D46072000
    WindFieldComponent.GetSimulatePause = 0x6D46072000
    WindFieldComponent.SetAmbientWind = 0x6D46072000
    WindFieldComponent.SetNoiseTexture = 0x6D46072000
    WindFieldComponent.SetPositionOffset = 0x6D46072000
    WindFieldComponent.SetSimulatePause = 0x6D46072000
    WindFieldComponent.SetTypeInfo = 0x6D46072000
    WindMotorComponent.AddMotorCylinder = 0x6D46072000
    WindMotorComponent.AddMotorDirectional = 0x6D46072000
    WindMotorComponent.AddMotorMoving = 0x6D46072000
    WindMotorComponent.AddMotorOmni = 0x6D46072000
    WindMotorComponent.AddMotorPressure = 0x6D46072000
    WindMotorComponent.AddMotorVortex = 0x6D46072000
    WindMotorComponent.RemoveMotor = 0x6D46072000
    WindMotorComponent.UpdateMotor = 0x6D46072000

class FoliageCluster:
    LightmapID = 0x30
    LightmapOffsets = 0x40
    LightmapScales = 0x58
    WorldBB = 0x18

class FoliageClusterCompressed:
    LightMapScaleMax = 0x88
    LightMapScaleMin = 0x78
    LightMapScales = 0x98
    PitchRolls = 0x48
    Positions = 0x18
    Scales = 0x60
    Yaws = 0x30

class FoliageComponent:
    Clusters = 0x2A0
    Compressed = 0x2B8
    EnableInteraction = 0x339
    IsCompressed = 0x337
    IsDisableXRay = 0x338
    IsLightmapConst = 0x33B
    LastLodBillboard = 0x334
    LodDistance0 = 0x320
    LodDistance1 = 0x328
    LodMeshTransitionTime = 0x330
    LodTransition0 = 0x324
    LodTransition1 = 0x32C
    ReceiveDecals = 0x33A
    TerrainOverlayBlend = 0x33C
    Version = 0x340

class FollowController:
    FollowRotation = 0x98
    GlobalOffset = 0x9C
    LocalOffset = 0xCC
    Pitch = 0x108
    Position = 0xFC
    Roll = 0x110
    Target = 0x90
    Yaw = 0x10C

class FollowMotor:
    Direction = 0x128
    LastTarget = 0x168
    Target = 0x170
    TargetPos = 0x110
    TargetPosOffset = 0x11C
    TargetTransform = 0x134

class FollowPlacer:
    AngularSpeed = 0x138
    BoneName = 0xC0
    Direction = 0x168
    EnableFaceCheck = 0xE0
    EnableInertia = 0x128
    EnableRotate = 0x130
    EnableUpPos = 0xF0
    EnableZoomSmooth = 0x1B0
    FaceThreshold = 0xE4
    FocusDiffTol = 0x12C
    FocusPos = 0x118
    Fov = 0x154
    InertiaCoeff = 0x124
    Mover = 0xE8
    PitchMax = 0x108
    PitchMin = 0x104
    PrevTargetPos = 0x13C
    Roll = 0x158
    Target = 0x178
    TargetDiffTol = 0x134
    TargetPos = 0x148
    TargetPosOffset = 0x15C
    TargetTransform = 0x180
    UseBonePosition = 0xB8
    ZoomHalfLife = 0xF8
    ZoomMax = 0x100
    ZoomMin = 0xFC

class FovEasingInfo:
    easingEndFov = 0xC
    easingFinished = 0x10
    easingFinishedNextFrame = 0x11

class FpsMover:
    EscapedTime = 0x164
    MoveTime = 0x160
    PitchMax = 0x174
    PitchMin = 0x170

class FpsPlacer:
    AdditiveAffiliatedFovOffset = 0x124
    AdditiveBoneName = 0xA8
    AdditiveFovOffset = 0x120
    AdditiveRollOffset = 0x12C
    AffiliatedFov = 0x11C
    ApplyPivotRotate = 0x164
    BasePosition = 0x134
    BoneName = 0x90
    Direction = 0x158
    Easing = 0xD0
    EasingRuntimeInfo = 0xD8
    Fov = 0x118
    Mover = 0xC8
    PitchMax = 0x114
    PitchMin = 0x110
    PivotDist = 0x130
    PivotOffset = 0x14C
    PositionOffset = 0x140
    Roll = 0x128
    Target = 0x168
    UseBonePosition = 0x88

class FramePlacer:
    Cursor = 0xB0
    Duration = 0xB8
    Finished = 0xAD
    Origin = 0xA0
    Suspend = 0xAC
    SuspendCursor = 0xB4

class FreeviewCameraController:
    KeepAnchor = 0x12D
    KeepRoll = 0x12C
    VirtualCamera = 0x98

class FreeviewPlacer:
    AffiliatedFov = 0xDC
    Direction = 0xAC
    DirectionHalflife = 0xC4
    DirectionTolerance = 0xC8
    Finished = 0x88
    FocusHalflife = 0xA4
    FocusPos = 0x8C
    FocusTolerance = 0xA8
    Fov = 0xCC
    FovHalflife = 0xD4
    FovTolerance = 0xD8
    PrevDirection = 0xB8
    PrevFocusPos = 0x98
    PrevFov = 0xD0

class Functions to convert between Python values and C structs:
    Python bytes objects are used to hold the data representing the C struct
and also as format strings (explained below) to describe the layout of data
in the C struct.

The optional first format char indicates byte order, size and alignment:
  @: native order, size & alignment (default)
  =: native order, std. size & alignment
  <: little-endian, std. size & alignment
  >: big-endian, std. size & alignment
  !: same as >

The remaining chars indicate types of args and must match exactly;
these can be preceded by a decimal repeat count:
  x: pad byte (no data); c:char; b:signed byte; B:unsigned byte;
  ?: _Bool (requires C99; if not available, char is used instead)
  h:short; H:unsigned short; i:int; I:unsigned int;
  l:long; L:unsigned long; f:float; d:double; e:half-float.
Special cases (preceding decimal count indicates length):
  s:string (array of char); p: pascal string (with count byte).
Special cases (only available in native format):
  n:ssize_t; N:size_t;
  P:an integer type that is wide enough to hold a pointer.
Special case (not in native mode unless 'long long' in platform C):
  q:long long; Q:unsigned long long
Whitespace between formats is ignored.

The variable struct.error is an exception raised on errors. = 0x10

class GarmentInternalLineCons:
    P3 = 0xC
    RestAngle = 0x10

class GarmentMesh:
    Indices = 0x30
    PosMat = 0x18

class GarmentPhysicsProperty:
    Density = 0x20
    Friction = 0x24
    InternalDamping = 0x1C
    StretchStiffness = 0x18

class GarmentPiece:
    InternalShapes = 0x28
    Layer = 0x20

class GarmentPieceInfo:
    IndexEnd = 0xC

class GarmentRawResource:
    ColShapes = 0xE0
    GarmentMesh = 0x80
    GarmentPieces = 0x38
    GarmentRenderInfo = 0x140
    GarmentSeamLines = 0x68
    MaxDistance = 0xC8
    PhysicsProperties = 0x50

class GarmentResource:
    CollisionSkinningMesh = 0x188
    DihBends = 0xA8
    DihBendsIndex = 0x88
    GarmentRenderMeshs = 0x218
    GarmentSkinningMesh = 0x110
    IsoBends = 0x70
    IsoBendsIndex = 0x50
    Masks = 0xF8
    SeamGroup = 0xE0
    SeamGroupIndex = 0xC0
    StaticBound = 0x200
    Stretchs = 0x38
    StretchsIndex = 0x18

class GarmentSeamLine:
    P1s = 0x18

class GarmentSimulationProperty:
    BindingDistance = 0x30
    BindingStiffness = 0x34
    BindingTolerance = 0x2C
    BodyCollisionDetectionScale = 0x18
    BodyCollisionThickness = 0x14
    DiheralBendStiffness = 0xC
    EnableBinding = 0x28
    EnableSelfCollision = 0x1C
    IsometricBendStiffness = 0x10
    SelfCollisionDetectionScale = 0x24
    SelfCollisionThickness = 0x20

class GarmentSoftSkinningMeshData:
    Bones = 0x60
    Indices = 0x18
    SkinIndices = 0x48
    SkinWeights = 0x30

class GeometryCacheInfo:
    EndFrameTime = 0x10
    GeometryCacheTree = 0x20
    IsConstantTopology = 0x18
    SecondsPerFrame = 0x14
    StartFrameTime = 0xC

class GeometryCacheNode:
    Children = 0x20
    Parent = 0x18

class GlobalEnvVolumeComponent:
    ActiveVolume = 0xC48
    AutoExposureMeterMask = 0xB00
    BloomDirtMask = 0xB10
    BlueNoiseMap = 0xBA0
    Cloud3DNoiseMap = 0xB30
    CloudDetail3DNoiseMap = 0xB40
    CloudWeatherMap = 0xB20
    DetailFogAlbedoA = 0xBEC
    DetailFogAlbedoB = 0xBF8
    DetailFogCoverage = 0xBD0
    DetailFogCutoff = 0xBE8
    DetailFogMaxHeight = 0xBE4
    DetailFogMinHeight = 0xBE0
    FadeOutTime = 0xC3C
    FadeOutType = 0xC40
    FogWeatherMap = 0xBC0
    LenFlares = 0xB68
    LensFadeTime = 0xB60
    LensMap = 0xB50
    MoonMap = 0xBB0
    MusicEventParam = 0xC08
    NoiseEventParam = 0xC20
    RainNormalMap = 0xB80
    SkinProfileMap = 0xB90
    VolumeScale = 0xC38

class GraphicsSettings:
    BillboardDistance = 0x54
    CSM_Distance0 = 0x1C
    CSM_Distance1 = 0x20
    CSM_Distance2 = 0x24
    CSM_Distance3 = 0x28
    CSM_FilterRange0 = 0x2C
    CSM_FilterRange1 = 0x30
    CSM_FilterRange2 = 0x34
    CSM_FilterRange3 = 0x38
    CSM_Layers = 0x3C
    CandelaProbeSH3 = 0xB4
    CandelaTransparencyGI = 0xB5
    DeferredLighting = 0xA
    EnableClipPrePass = 0x83
    EnableColorGrading = 0xC
    EnableCustomDepthStencil = 0x8C
    EnableFoliageIndirect = 0x95
    EnableGPUCloth = 0x94
    EnableGPUParticle = 0x8E
    EnableOcclus = 0x8D
    EnableReport = 0x80
    EnableSkinCache = 0xB0
    EnableTessellation = 0xB6
    EnableVisualGraph = 0x92
    FontEffect = 0x81
    FontVerticalSpacing = 0x88
    ForwardCSM = 0xB3
    GPUParticleBufferSize = 0x90
    HighDynamicRangeRender = 0xB
    IsRoughnessLevelPow2 = 0xB2
    LodDistance1 = 0x40
    LodDistance2 = 0x44
    LodDistance3 = 0x48
    LodMeshFadeTime = 0x60
    MaxDetailLevel = 0x7E
    MaxShaderLodLevel = 0xF
    MinShaderLodLevel = 0xE
    MipFocusDistance = 0x58
    MipMinLevel = 0x7F
    MipResistDistance = 0x5C
    MipUpdateFrames = 0x7C
    NearPlane = 0x84
    PlanarShadowFilterInfo = 0x64
    PlanarShadowTMax = 0x70
    PlanarShadowTraceDistance = 0x6C
    SDFVolumeSize0 = 0x4C
    SDFVolumeSize1 = 0x50
    SceneUIFilterInfo = 0x68
    ScreenScale = 0x18
    ShaderCacheDirectory = 0x98
    ShaderMacros = 0xB8
    Shaders = 0xD0
    ShadowMapSize = 0x10
    ShadowYClip = 0x14
    SunLightShadow = 0xD
    SunShadowDistanceFar = 0x78
    SunShadowDistanceNear = 0x74
    SupportMipDistance = 0x82
    UseOctahedronMap = 0xB1
    VisualGraphUseTextureBuffer = 0x93

class GroundPosRes:
    GroundPos = 0x18
    InPos = 0xC
    Status = 0x24

class GroupFollowController:
    Aspect = 0xE0
    DistanceRange = 0xD8
    FieldOfView = 0xC0
    MaxArea = 0xCC
    MinArea = 0xC4
    Pitch = 0xB4
    Position = 0xA8
    Roll = 0xBC
    Speed = 0xD4
    Targets = 0x90
    Yaw = 0xB8

class Gu:
    GeometryUnion = 0x1
    GeometryUnion = 0x2
    GeometryUnion = 0x3
    GeometryUnion = 0x4
    GeometryUnion = 0x5
    GeometryUnion = 0x6

class HFFluidRenderer:
    FFTSpeed = 0xF8

class HeightmapComponent:
    AccumulationSpeed = 0xDC
    BaseMapTileSize = 0xBC
    DetailMapTileSize = 0xC0
    EnableVSHeightmap = 0xD4
    FarTessDistance = 0xC8
    FarTriPerMeter = 0xD0
    MaxSnowLocalHeight = 0xD8
    NearTessDistance = 0xC4
    NearTriPerMeter = 0xCC
    TextureSize = 0xBA

class IAnimator:
    Doll = 0x80
    PropertyName = 0x88

class IAppearanceComponent:
    BlendInOutTime = 0xF4
    BlendRatio = 0xEC
    DepthOffset = 0xF0
    IsReadyToAppear = 0xE7
    Status = 0xE9
    Storyboard = 0xA8
    SyncPrimitives = 0xE8
    WorldBound = 0xB0

class IArea:
    AreaId = 0x60
    ConnectedAreas = 0xD8
    Entities = 0xB0
    IsFocusing = 0x66
    IsInWorld = 0x65
    IsIndoor = 0x64
    IsRootArea = 0x67
    Level = 0xA0
    Space = 0xA8
    Storyboard = 0x100
    Title = 0x108
    Transform = 0x68
    World = 0x98

class ICamera:
    Blurriness = 0x354
    EnableDepthOfField = 0x301
    Far = 0x340
    FarTransitionRegion = 0x350
    FieldOfView = 0x338
    FocalDistance = 0x344
    FocalRegion = 0x348
    IsOrthographic = 0x300
    Near = 0x33C
    NearTransitionRegion = 0x34C
    OrthoScale = 0x334
    ViewMatrix = 0x304
    VirtualCameraManager = 0x2F8

class IComponent:
    Parent = 0x6D46046000

class IController:
    ViewControl = 0x60

class IEntity:
    Action = 0x6D46046000
    Anchor = 0x180
    AnimatorDestructor = 0x6D46046000
    Appearance = 0x6D46046000
    Area = 0x88
    Audio = 0x6D46046000
    AudioObject = 0x6D46046000
    Billboard = 0x6D46046000
    BillboardLockY = 0x6D46046000
    CharCtrl = 0x6D46046000
    ClothSimulator = 0x6D46046000
    Constraints = 0x100
    Decal = 0x6D46046000
    DeprecatedBit1 = 0x6D46046000
    Destruct = 0x6D46046000
    DetailLevel = 0x12C
    DockingMark = 0x6D46046000
    EnvVolume = 0x6D46046000
    Fading = 0x6D46046000
    Filter = 0x6D46046000
    Heightmap = 0x6D46046000
    InSkyBoxDistance = 0x1B0
    InSkyBoxPitch = 0x1AC
    InSkyBoxYaw = 0x1A8
    InheritVirtualColor = 0x12A
    IsAnimated = 0x1B4
    IsBillboard = 0x129
    IsCastDynamicShadow = 0x1B6
    IsDisableCollision = 0x6D46046000
    IsDisableFakeCmd = 0x12B
    IsDissolving = 0x6D46046000
    IsInSkyBox = 0x12D
    IsInWorld = 0x6D46046000
    IsLevelProxy = 0x1B5
    IsMovable = 0x6D46046000
    IsOutlined = 0x6D46046000
    IsOverrideLodThreshold = 0x6D46046000
    IsPointCloudLight = 0x1B8
    IsReadyToAppear = 0x6D46046000
    IsReceiveDynamicShadow = 0x1B7
    IsReflectionVisible = 0x6D46046000
    IsRenderStateChanged = 0x6D46046000
    IsResourceReady = 0x6D46046000
    IsSDFGen = 0x1B9
    IsVisible = 0x128
    IsWarmingUp = 0x6D46046000
    IsWholeLevelVisible = 0x6D46046000
    Level = 0x90
    LevelProxy = 0x6D46046000
    Light = 0x6D46046000
    LightProbe = 0x6D46046000
    LoadResourceExtend = 0x1BC
    LodThreshold = 0x174
    NavigateGadget = 0x6D46046000
    NavigateObstacle = 0x6D46046000
    PointCloud = 0x6D46046000
    PoseSender = 0x6D46046000
    Primitives = 0xD0
    Ragdoll = 0x6D46046000
    ReflectionProbe = 0x6D46046000
    RigidBodies = 0xE8
    SHVolume = 0x6D46046000
    SceneGuider = 0x6D46046000
    ShaderGraphOverrideMaterialInfo = 0x190
    Skeleton = 0x6D46046000
    SnowTrail = 0x6D46046000
    Storyboard = 0x6D46046000
    Tach = 0x6D46046000
    Tag = 0x6D46046000
    TechParam = 0x134
    TechParam2 = 0x140
    TechState = 0x6D46046000
    TintColor1 = 0x150
    TintColor2 = 0x15C
    TintColor3 = 0x168
    Token = 0x124
    Transform = 0x58
    TransformUpdate = 0x118
    UI = 0x6D46046000
    VisibilityCube = 0x6D46046000
    Volume = 0x6D46046000
    World = 0x98

class IEnvVolumeComponent:
    FadeTime = 0x10C

class IGameplay:
    Controller = 0x50
    HoldingInstance = 0x68
    IsSimulated = 0x4D
    IsStarted = 0x4C
    LoadingShelter = 0x80
    Player = 0x58
    Scenario = 0x60

class ILevel:
    Anchor = 0xC8
    AppearableReadyCount = 0x11C
    AppearableTotalCount = 0x118
    DefaultLoad = 0xD6
    EnableProxy = 0xD7
    FadeOutTime = 0x154
    FadeOutType = 0x158
    IsInWorld = 0xD4
    IsProxied = 0x164
    IsReadyToAppear = 0xD5
    Music = 0x120
    NeedPhysics = 0xD8
    Noise = 0x138
    Priority = 0x150
    ProxyDistance = 0x15C
    ProxyResistDistance = 0x160
    RootArea = 0xE0
    Storyboard = 0x110
    SubAreas = 0xE8
    Title = 0x78
    Transform = 0x98
    World = 0x90

class ILevelProxy:
    Target = 0x60

class ILightComponent:
    CastDynamicShadow = 0xB0
    CastShadow = 0x9D
    CastVolumetricShadow = 0xA4
    Channel = 0xD8
    Enable = 0x98
    EnableVolumetricFog = 0x9F
    FlareNodes = 0xB8
    IsBake = 0x9A
    IsBakeGIOnly = 0x9B
    IsDynamic = 0x99
    IsHybridBake = 0x9C
    IsImportant = 0xB2
    LensFadeTime = 0xD0
    OccluId = 0xD4
    PBRQuality = 0x9E
    SSSTranslucency = 0xA5
    ShadowBias = 0xA8
    ShadowQuality = 0xB1
    ShadowSlopeBias = 0xAC
    VolumetricScatteringIntensity = 0xA0

class INavigateGadget:
    AreaID = 0x64
    Radius = 0x60

class INavigateMap:
    AutoLoader = 0xA8
    IsResourceReady = 0xD8
    MapDisplayer = 0xB0
    MapEntity = 0xA0
    MaxAvoidNeighbours = 0x98
    OriginOffset = 0x80
    Resource = 0xC8
    ShowDebug = 0xD9

class INavigateObstacle:
    AreaID = 0xC8
    AutoEnterMainMap = 0xCC
    Enable = 0x68
    NaviMap = 0xD0
    OffsetTransform = 0x98
    ShapePath = 0x70
    ShapeRes = 0x88

class INavigateTrigger:
    Radius = 0x60

class INavigatorComponent:
    AllowedError = 0x170
    AngularSpeed = 0xEC
    ApplyYawToEnt = 0x163
    AutoEnterMainMap = 0x16E
    AwayFromCorner = 0x16C
    CollisionQueryRange = 0x104
    CtrlEntY = 0x161
    CtrlEntYaw = 0x162
    CurAreaID = 0x6D48E27F50
    DebugDrawer = 0xE0
    DebugOutput = 0x16F
    DisableCollisionRecover = 0x101
    DistToCorner = 0x168
    EnableObstacle = 0x100
    EnableOptimize = 0x160
    EnableSmoothPath = 0x108
    IsMoving = 0x16D
    Map = 0x190
    MapCell = 0x148
    MapPos = 0x130
    MoveSpeed = 0xE8
    NavigateFlag = 0x164
    ObstacleClass = 0xF8
    ObstacleRadius = 0xFC
    OnRoad = 0xF4
    Pos = 0x118
    TargetPosUpdateFreq = 0x110
    TargetYawDampScale = 0x10C
    UseSeparate = 0x114
    Yaw = 0xF0

class IObject:
    Class = 0x6D46046000
    Flags = 0x6D46046000
    IsPersist = 0x6D46046000
    Name = 0x6D46046000

class IPhysicsCollidableBody:
    DebugDrawer = 0x108
    DebugOutput = 0x112
    EnableContactNotify = 0x110
    PassiveMode = 0x111

class IPhysicsDynamicRB:
    IsUnderSync = 0x15A
    StateSender = 0x128
    StateSyncer = 0x130

class IPhysicsSpace:
    CDBodyDebugDrawers = 0x198
    LeavedBodyDelayCache = 0x180
    ShowCDBodyDebugDrawer = 0x1C0

class IPhysicsSpaceBody:
    Entity = 0xB0
    Space = 0xA8

class IPhysicsSyncBodySenderAndSyncerBase:
    DebugOutput = 0x8D
    Debuger = 0x80
    Space = 0x78

class IPhysicsSyncBodySenderAndSyncerDebugBase:
    Enable = 0x90
    Storyboard = 0x80

class IPrimitiveComponent:
    CustomDepthStencilValue = 0x1BA
    CustomRenderSet = 0x1B0
    CustomShadowBias = 0x1C0
    CustomStencil = 0x1B8
    InheritClipFlag = 0x1E0
    InheritTwoSidedFlag = 0x1E1
    IsCastDynamicShadow = 0x1BB
    IsIgnoreInheritFlag = 0x1E8
    IsInSpace = 0x1E4
    IsPointCloudLight = 0x1E7
    IsReadyToAppear = 0x1E3
    IsReceiveDynamicShadow = 0x1BC
    IsResourceReady = 0x1E2
    IsVisible = 0x1E5
    IsWarmingUp = 0x1E6
    LocalBound = 0x150
    OccluId = 0x1C8
    OcclusFlag = 0x1C5
    OverrideMaterial = 0x1D0
    PrimLocalBound = 0x180
    PrimWorldBound = 0x198
    RenderInCustomDepthPass = 0x1B9
    Resource = 0x140
    WorldBound = 0x168

class IProperty:
    Flags = 0x6D46046000
    Index = 0x6D46046000
    Name = 0x6D46046000
    Offset = 0x6D46046000
    OperationType = 0x6D46046000
    Outer = 0x6D46046000
    PropertyType = 0x6D46046000
    StorageType = 0x6D46046000
    Type = 0x6D46046000

class ISkeletonComponent:
    IsReady = 0x80

class IStoryboard:
    Elapsed = 0x184
    EnableSleep = 0x199
    FixedDelta = 0x188
    FrameLimit = 0x18C
    IsPlaying = 0x198
    ParentStoryboard = 0x178
    SleepFrameLimit = 0x190
    Speed = 0x180

class IType:
    Base = 0x6D46046000
    Category = 0x6D46046000
    Flags = 0x6D46046000
    KeyType = 0x6D46046000
    Name = 0x6D46046000
    Properties = 0xE8
    PropertyCount = 0x6D46046000
    Size = 0x6D46046000
    Sub = 0x6D46046000
    ValueType = 0x6D46046000

class IWorld:
    ActiveLevels = 0x118
    Anchor = 0x1E8
    AppearableReadyCount = 0x1F8
    AppearableTotalCount = 0x1F4
    DefaultLevel = 0x110
    EnvVolume = 0x1A8
    FilePath = 0x200
    Gameplay = 0x108
    InterestPoint = 0x148
    IsInitialized = 0xDC
    IsLockingAudio = 0xDF
    IsLockingProxy = 0xDE
    IsReadyToAppear = 0xDD
    Levels = 0xE0
    NavigateMap = 0x1C8
    ObservationPoints = 0x188
    PSVolumeSpace = 0x1C0
    PhysicsSpace = 0x1B0
    PreloadDistances = 0x170
    PreloadPivots = 0x158
    PvsSpace = 0x1B8
    Scene = 0x140
    Storyboard = 0x1A0
    SubNavigateMaps = 0x1D0
    Title = 0xC0
    Version = 0xD8

class IconSetting:
    Height = 0x1C
    Width = 0x18

class IdentifiedResource:
    Guid = 0x18
    InitialPose = 0x28

class ImageComponent:
    CacheCount = 0xD0
    ImageGuid = 0xD8

class Implementation module for SSL socket operations:
    See the socket module
for documentation. = 0x10

class ImpostorInfo:
    Distance = 0xC

class IsometricBendCons:
    Q = 0x10

class LerpBlender:
    BlendTime = 0xB8
    BlendType = 0xC4
    DirectTransit = 0xC2
    Escaped = 0xBC
    Finished = 0xC0
    InvalidLastInfo = 0xC1
    LastDirection = 0xA4
    LastFov = 0xB0
    LastRoll = 0xB4
    LastTargetPos = 0x98

class LevelVolumeComponent:
    EnableOffset = 0x200
    LevelKey = 0x1E8

class LightProbeComponent:
    AttenDistance = 0xDC
    Enable = 0xD8
    EnableTimeOfDay = 0xDA
    IBL_Diffuse = 0xE4
    IBL_SH = 0x12C
    InShadow = 0xE0
    IsDefault = 0xD9
    TimeOfDays = 0x1A0

class LightProbeKeyFrameData:
    IBL_SH = 0x4C

class LinearMover:
    CameraMoveTime = 0x178
    EscapedTime = 0x164
    FocusMoveTime = 0x174
    FocusSpeed = 0x168
    InvalidMoveTime = 0x170
    MoveSpeed = 0x16C
    MoveTime = 0x160

class LoadRepositoryFail:
    tag = 0x40

class LodGroupComponent:
    LodStartLevel = 0x2C0
    OverrideLodDistance = 0x2A8
    OverrideTransitionDistance = 0x2A0
    SubModels = 0x288

class LookAtController:
    Damping = 0xD8
    DeadZone = 0xC8
    Offset = 0xBC
    Pitch = 0xE4
    Position = 0xB0
    SoftZone = 0xD0
    Target = 0x90
    TargetBone = 0x98
    Yaw = 0xE8

class Lua:
    LuaChef.LuaCreateType = 0x6D46072000
    LuaDispatcher.LuaCreateType = 0x6D46072000
    LuaMType.LuaCreateType = 0x6D46072000
    LuaNative.LuaCreateType = 0x6D46072000
    LuaVM.Destroy = 0x6D46072000
    LuaVM.EnableProfiler = 0x6D46072000
    LuaVM.Run = 0x6D46072000

class LuaBinding:
    LuaObjects.LuaCreateType = 0x6D46072000

class MachineInfo:
    Addresses = 0x48
    Applications = 0x60
    MemoryPageSize = 0x3C
    MemoryTotal = 0x40
    Online = 0x31
    ProcessId = 0x38
    ProcessorCore = 0x34
    ProcessorGroup = 0x32
    Remote = 0x30
    User = 0x18

class MaskMapSlot:
    IsUsing = 0x10

class MaterialLayer:
    Parameters = 0x30
    ShaderName = 0x18

class MeshInfo:
    CastReflection = 0xD
    CastShadow = 0xC
    DetailCollision = 0xE

class MeshLodInfo:
    LodScreenSize = 0x20

class Messiah:
    ActionComponent.Play = 0x6D46072000
    ActionComponent.PlayNow = 0x6D46072000
    ActionComponent.Stop = 0x6D46072000
    ActionComponent.StopAt = 0x6D46072000
    AnimatorDestructor.RevertBreak = 0x6D46072000
    AnimatorDestructor.TriggerBreak = 0x6D46072000
    AudioObjectComponent.ExecuteActionOnEvent = 0x6D46072000
    AudioObjectComponent.PostEvent = 0x6D46072000
    AudioObjectComponent.PostMIDIOnEvent = 0x6D46072000
    AudioObjectComponent.SeekOnEvent = 0x6D46072000
    AudioObjectComponent.SetGameObjectOnMultiplePositions = 0x6D46072000
    AudioObjectComponent.SetRTPCValue = 0x6D46072000
    AudioObjectComponent.SetScalingFactor = 0x6D46072000
    AudioObjectComponent.SetSwitch = 0x6D46072000
    AudioObjectComponent.StopAll = 0x6D46072000
    AudioObjectComponent.StopMIDIOnEvent = 0x6D46072000
    BaseEnvVolumeComponent.ForceRefreshTimeOfDay = 0x6D46072000
    BaseEnvVolumeComponent.GetCustomParameter = 0x6D46072000
    BaseEnvVolumeComponent.SetCustomParameter = 0x6D46072000
    BatchComponent.AddInstance = 0x6D46072000
    BatchComponent.ApplyCustomMaterial = 0x6D46072000
    BatchComponent.DelInstance = 0x6D46072000
    BatchComponent.GetInstanceIndex = 0x6D46072000
    BatchComponent.ResetCustomMaterial = 0x6D46072000
    BatchComponent.UpdateInstance = 0x6D46072000
    BoardComponent.AddImageLayer = 0x6D46072000
    BoardComponent.AddImageLayerComposition = 0x6D46072000
    BoardComponent.AddTextLayer = 0x6D46072000
    BoardComponent.GetImageLayerCount = 0x6D46072000
    BoardComponent.GetImageLayerPercent = 0x6D46072000
    BoardComponent.GetTextLayerCount = 0x6D46072000
    BoardComponent.InitBubbleBackground = 0x6D46072000
    BoardComponent.InitBubbleBkgWithoutTail = 0x6D46072000
    BoardComponent.InitBubbleContent = 0x6D46072000
    BoardComponent.SetBubbleFontName = 0x6D46072000
    BoardComponent.SetBubbleOffset = 0x6D46072000
    BoardComponent.SetImageLayerCapInsets = 0x6D46072000
    BoardComponent.SetImageLayerOffset = 0x6D46072000
    BoardComponent.SetImageLayerPercent = 0x6D46072000
    BoardComponent.SetImageLayerVerticalPercent = 0x6D46072000
    BoardComponent.SetImageLayerVisibility = 0x6D46072000
    BoardComponent.SetImageLayerWrap = 0x6D46072000
    BoardComponent.SetTextLayerOffset = 0x6D46072000
    BoardComponent.SetTextLayerVisibility = 0x6D46072000
    BoardComponent.UpdateFontName = 0x6D46072000
    BoardComponent.UpdateFontSize = 0x6D46072000
    BoardComponent.UpdateImageComposition = 0x6D46072000
    BoardComponent.UpdateImageLayerAlpha = 0x6D46072000
    BoardComponent.UpdateImageLayerBlend = 0x6D46072000
    BoardComponent.UpdateImageLayerComposition = 0x6D46072000
    BoardComponent.UpdateImageLayerOverlayColor = 0x6D46072000
    BoardComponent.UpdateImageLayerResource = 0x6D46072000
    BoardComponent.UpdateSpeech = 0x6D46072000
    BoardComponent.UpdateTextLayerAnchorPoint = 0x6D46072000
    BoardComponent.UpdateTextLayerColor = 0x6D46072000
    BoardComponent.UpdateTextLayerContent = 0x6D46072000
    BoardComponent.UpdateTextLayerScale = 0x6D46072000
    ClientManipulator.ClearSelections = 0x6D46072000
    ClientManipulator.SelectEntity = 0x6D46072000
    ClientManipulator.SelectGizmo = 0x6D46072000
    ClientPlayer.AddAffiliatedCamera = 0x6D46072000
    ClientPlayer.GetAffiliatedCamera = 0x6D46072000
    ClientPlayer.QueryAffiliatedCamera = 0x6D46072000
    ClientPlayer.RemoveAffiliatedCamera = 0x6D46072000
    ClientScenario.BeginLoading = 0x6D46072000
    ClientScenario.ClearLinkedWorlds = 0x6D46072000
    ClientScenario.DeleteLinkedWorld = 0x6D46072000
    ClientScenario.DeleteShowRoom = 0x6D46072000
    ClientScenario.EndLoading = 0x6D46072000
    ClientScenario.LoadWorld = 0x6D46072000
    ClientScenario.LoadWorldLevels = 0x6D46072000
    ClientScenario.MakeLinkedWorld = 0x6D46072000
    ClientScenario.MakeShowRoom = 0x6D46072000
    ClientScenario.MakeShowRoom2 = 0x6D46072000
    ClientScenario.MakeWorld = 0x6D46072000
    ClientScenario.OverrideDeferredLevels = 0x6D46072000
    ClothModelComponent.LoadClothModel = 0x6D46072000
    ClothSimulatorComponent.BeginTeleport = 0x6D46072000
    ClothSimulatorComponent.CreateExternalPlaneColShape = 0x6D46072000
    ClothSimulatorComponent.CreateExternalSphereColShape = 0x6D46072000
    ClothSimulatorComponent.DeleteExternalPlaneColShape = 0x6D46072000
    ClothSimulatorComponent.DeleteExternalSphereColShape = 0x6D46072000
    ClothSimulatorComponent.EndTeleport = 0x6D46072000
    ClothSimulatorComponent.GetCouplingConfig = 0x6D46072000
    ClothSimulatorComponent.GetCouplingRigidBodys = 0x6D46072000
    ClothSimulatorComponent.GetSphereColShapes = 0x6D46072000
    ClothSimulatorComponent.Reset = 0x6D46072000
    ClothSimulatorComponent.SetCouplingConfig = 0x6D46072000
    ClothSimulatorComponent.SetCouplingRigidBodys = 0x6D46072000
    ClothSimulatorComponent.SetExternalPlaneColShape = 0x6D46072000
    ClothSimulatorComponent.SetExternalSphereColShape = 0x6D46072000
    ConstraintDataWrapper.SetActorIds = 0x6D46072000
    ConstraintDataWrapper.SetLocalFrames = 0x6D46072000
    ConstraintDataWrapper.SetToTumblerConstraint = 0x6D46072000
    ConstraintDataWrapper.SetToWheelSuspensionConstraint = 0x6D46072000
    DamageParams.IsDamageValid = 0x6D46072000
    DestructibleActor.GetAllBondHealth = 0x6D46072000
    DestructibleActor.GetChunkHealth = 0x6D46072000
    DestructibleActor.GetChunkIndex = 0x6D46072000
    DestructibleActor.GetInitPose = 0x6D46072000
    DestructibleActor.GetLargestBondHealth = 0x6D46072000
    DestructibleActor.GetSmallestBondHealth = 0x6D46072000
    DestructibleActor.IsLeafNode = 0x6D46072000
    DestructibleFamily.ApplyFaimlySyncEvents = 0x6D46072000
    DestructibleFamily.CanDoDamage = 0x6D46072000
    DestructibleFamily.CreateFamilySyncEvents = 0x6D46072000
    DestructibleFamily.Damage = 0x6D46072000
    DestructibleFamily.ExportDestructShapes = 0x6D46072000
    DestructibleFamily.GetBlastBonds = 0x6D46072000
    DestructibleFamily.GetBlastChunkByIndex = 0x6D46072000
    DestructibleFamily.GetBlastChunks = 0x6D46072000
    DestructibleFamily.GetChildChunkIndex = 0x6D46072000
    DestructibleFamily.GetDamageMaterial = 0x6D46072000
    DestructibleFamily.GetInitPose = 0x6D46072000
    DestructibleFamily.IsBroken = 0x6D46072000
    DestructibleFamily.ResetToOriginState = 0x6D46072000
    DestructibleFamily.SetInitPose = 0x6D46072000
    DestructibleFamilySyncEvents.CreateString = 0x6D46072000
    DestructibleFamilySyncEvents.ResetFromString = 0x6D46072000
    DestructibleModelComponent.GetChunkPoses = 0x6D46072000
    DestructibleModelComponent.GetChunkVisiblities = 0x6D46072000
    DestructibleModelComponent.ShiftOrigin = 0x6D46072000
    FoliageComponent.AddFoliagesRuntime = 0x6D46072000
    FoliageComponent.ClearFoliagesRuntime = 0x6D46072000
    GlobalEnvVolumeComponent.HandleBackgroundMusic = 0x6D46072000
    GlobalEnvVolumeComponent.OnFlareChanged = 0x6D46072000
    GlobalEnvVolumeComponent.SetBackgroundMusicKeyOff = 0x6D46072000
    GlobalEnvVolumeComponent.SetBackgroundMusicPaused = 0x6D46072000
    GlobalEnvVolumeComponent.SetBackgroundMusicPlayStop = 0x6D46072000
    GlobalEnvVolumeComponent.SetBackgroundMusicVolumeRamp = 0x6D46072000
    GlobalEnvVolumeComponent.SetBackgroundMusicVolumeScale = 0x6D46072000
    ICamera.GetOrigin = 0x6D46072000
    ICamera.GetRayDirectionFromScreenPoint = 0x6D46072000
    ICamera.GetScreenPointFromWorldPoint = 0x6D46072000
    IEntity.AddPrimitive = 0x6D46072000
    IEntity.Attach = 0x6D46072000
    IEntity.DelPrimitive = 0x6D46072000
    IEntity.Detach = 0x6D46072000
    IEntity.EnterArea = 0x6D46072000
    IEntity.GetIsBillBoard = 0x6D46072000
    IEntity.GetIsDissolving = 0x6D46072000
    IEntity.GetIsOutlined = 0x6D46072000
    IEntity.GetPrimitiveWorldBound = 0x6D46072000
    IEntity.GetWorldBound = 0x6D46072000
    IEntity.LeaveArea = 0x6D46072000
    IEntity.SetAsRole = 0x6D46072000
    IEntity.SetIsBillBoard = 0x6D46072000
    IEntity.SetIsDissolving = 0x6D46072000
    IEntity.SetIsOutlined = 0x6D46072000
    IEntity.SetNoNearPlaneClip = 0x6D46072000
    IGameplay.AddHoldingInstance = 0x6D46072000
    IGameplay.ClearShelter = 0x6D46072000
    IGameplay.DelHoldingInstance = 0x6D46072000
    IGameplay.Move = 0x6D46072000
    IGameplay.ShelterLoadingItem = 0x6D46072000
    IGameplay.Stop = 0x6D46072000
    ILevel.AcquirePhysics = 0x6D46072000
    ILevel.DoEnterWorld = 0x6D46072000
    ILevel.DoLeaveGame = 0x6D46072000
    ILevel.DoLeaveWorld = 0x6D46072000
    ILevel.GetFoliageLodDistanceBias = 0x6D46072000
    ILevel.GetLodDistanceBias = 0x6D46072000
    ILevel.ReleasePhysics = 0x6D46072000
    ILevel.SetFoliageLodDistanceBias = 0x6D46072000
    ILevel.SetLodDistanceBias = 0x6D46072000
    IPhysicsBodyStateSender.SetSenderInterval = 0x6D46072000
    IPhysicsBodyStateSender.SetStateSender = 0x6D46072000
    IPhysicsBodySyncer.GetDelayedStateReceiver = 0x6D46072000
    IPhysicsBodySyncer.GetStateReceiver = 0x6D46072000
    IPhysicsCollidableBody.GetCanRideOn = 0x6D46072000
    IPhysicsCollidableBody.GetCollisionFilterData = 0x6D46072000
    IPhysicsCollidableBody.GetCollisionFilterInfo = 0x6D46072000
    IPhysicsCollidableBody.GetLocalBound = 0x6D46072000
    IPhysicsCollidableBody.GetMotionType = 0x6D46072000
    IPhysicsCollidableBody.GetShapeCount = 0x6D46072000
    IPhysicsCollidableBody.GetShapeIsQuery = 0x6D46072000
    IPhysicsCollidableBody.GetShapeIsSimulate = 0x6D46072000
    IPhysicsCollidableBody.GetShapeIsTrigger = 0x6D46072000
    IPhysicsCollidableBody.GetWorldPose = 0x6D46072000
    IPhysicsCollidableBody.GetWorldTransform = 0x6D46072000
    IPhysicsCollidableBody.SetCanRideOn = 0x6D46072000
    IPhysicsCollidableBody.SetCollisionFilterData = 0x6D46072000
    IPhysicsCollidableBody.SetCollisionFilterInfo = 0x6D46072000
    IPhysicsCollidableBody.SetShapeIsQuery = 0x6D46072000
    IPhysicsCollidableBody.SetShapeIsSimulate = 0x6D46072000
    IPhysicsCollidableBody.SetShapeIsTrigger = 0x6D46072000
    IPhysicsCollidableBody.SetWorldPose = 0x6D46072000
    IPhysicsDynamicRB.ApplyForceAtPosInWorldFrame = 0x6D46072000
    IPhysicsDynamicRB.ApplyForceInWorldFrame = 0x6D46072000
    IPhysicsDynamicRB.ApplyTorqueInWorldFrame = 0x6D46072000
    IPhysicsDynamicRB.ClearForce = 0x6D46072000
    IPhysicsDynamicRB.ClearTorque = 0x6D46072000
    IPhysicsDynamicRB.GetAngularAcc = 0x6D46072000
    IPhysicsDynamicRB.GetAngularDamping = 0x6D46072000
    IPhysicsDynamicRB.GetAngularVel = 0x6D46072000
    IPhysicsDynamicRB.GetCenterOfMass = 0x6D46072000
    IPhysicsDynamicRB.GetEnableCCD = 0x6D46072000
    IPhysicsDynamicRB.GetEnableGravity = 0x6D46072000
    IPhysicsDynamicRB.GetInertiaTensor = 0x6D46072000
    IPhysicsDynamicRB.GetLinearAcc = 0x6D46072000
    IPhysicsDynamicRB.GetLinearDamping = 0x6D46072000
    IPhysicsDynamicRB.GetLinearVel = 0x6D46072000
    IPhysicsDynamicRB.GetMass = 0x6D46072000
    IPhysicsDynamicRB.GetMassLocalPose = 0x6D46072000
    IPhysicsDynamicRB.GetMassLocalTrans = 0x6D46072000
    IPhysicsDynamicRB.GetMaxAngularSpeed = 0x6D46072000
    IPhysicsDynamicRB.GetMaxLinearSpeed = 0x6D46072000
    IPhysicsDynamicRB.GetSleepThreshold = 0x6D46072000
    IPhysicsDynamicRB.SetAngularDamping = 0x6D46072000
    IPhysicsDynamicRB.SetAngularVel = 0x6D46072000
    IPhysicsDynamicRB.SetEnableCCD = 0x6D46072000
    IPhysicsDynamicRB.SetEnableGravity = 0x6D46072000
    IPhysicsDynamicRB.SetKinematicTarget = 0x6D46072000
    IPhysicsDynamicRB.SetLinearDamping = 0x6D46072000
    IPhysicsDynamicRB.SetLinearVel = 0x6D46072000
    IPhysicsDynamicRB.SetMaxAngularSpeed = 0x6D46072000
    IPhysicsDynamicRB.SetMaxLinearSpeed = 0x6D46072000
    IPhysicsDynamicRB.SetMotionType = 0x6D46072000
    IPhysicsDynamicRB.SetSleepThreshold = 0x6D46072000
    IPhysicsDynamicRB.SetWorldTransform = 0x6D46072000
    IPhysicsSpace.GetFrame = 0x6D46072000
    IPhysicsSpace.GetGravity = 0x6D46072000
    IPhysicsSpace.SetFrame = 0x6D46072000
    IPhysicsSpace.SetGravity = 0x6D46072000
    IPhysicsSpace.ShiftOrigin = 0x6D46072000
    IPhysicsSpaceBody.EnterSpace = 0x6D46072000
    IPhysicsSpaceBody.IsInSpace = 0x6D46072000
    IPhysicsSpaceBody.LeaveSpace = 0x6D46072000
    IPhysicsSpaceBody.ShiftOrigin = 0x6D46072000
    IPrimitiveComponent.ChangeShaderGraphMaterial = 0x6D46072000
    IPrimitiveComponent.ChangeShaderGraphMaterialWithInheritMask = 0x6D46072000
    IPrimitiveComponent.GetIndexShaderMaterialFlag = 0x6D46072000
    IPrimitiveComponent.GetIndexShaderParameter = 0x6D46072000
    IPrimitiveComponent.GetOverrideMaterial = 0x6D46072000
    IPrimitiveComponent.GetShaderGraphMaterialFlags = 0x6D46072000
    IPrimitiveComponent.GetShaderGraphOriginParameter = 0x6D46072000
    IPrimitiveComponent.GetShaderGraphParameter = 0x6D46072000
    IPrimitiveComponent.GetShaderMaterialFlags = 0x6D46072000
    IPrimitiveComponent.GetShaderParameter = 0x6D46072000
    IPrimitiveComponent.HasIndexShaderDefineParameter = 0x6D46072000
    IPrimitiveComponent.HasShaderDefineParameter = 0x6D46072000
    IPrimitiveComponent.LoadResource = 0x6D46072000
    IPrimitiveComponent.NeedAppearance = 0x6D46072000
    IPrimitiveComponent.OverrideAllMaterial = 0x6D46072000
    IPrimitiveComponent.OverrideMaterialWithInheritMask = 0x6D46072000
    IPrimitiveComponent.ResetIndexShaderMaterialFlag = 0x6D46072000
    IPrimitiveComponent.ResetIndexShaderParameter = 0x6D46072000
    IPrimitiveComponent.ResetOverriddenMaterial = 0x6D46072000
    IPrimitiveComponent.ResetReadyToAppear = 0x6D46072000
    IPrimitiveComponent.ResetShaderGraphMaterial = 0x6D46072000
    IPrimitiveComponent.ResetShaderGraphMaterialFlags = 0x6D46072000
    IPrimitiveComponent.ResetShaderGraphParameter = 0x6D46072000
    IPrimitiveComponent.ResetShaderGraphParameters = 0x6D46072000
    IPrimitiveComponent.ResetShaderMaterialFlags = 0x6D46072000
    IPrimitiveComponent.ResetShaderParameter = 0x6D46072000
    IPrimitiveComponent.SetForceExcludeOcclusion = 0x6D46072000
    IPrimitiveComponent.SetForceIncludeOcclusion = 0x6D46072000
    IPrimitiveComponent.SetForceUpdateOcclusion = 0x6D46072000
    IPrimitiveComponent.SetHeightmap = 0x6D46072000
    IPrimitiveComponent.SetIndexShaderMaterialFlag = 0x6D46072000
    IPrimitiveComponent.SetIndexShaderParameter = 0x6D46072000
    IPrimitiveComponent.SetNoNearPlaneClip = 0x6D46072000
    IPrimitiveComponent.SetOccludeeEnable = 0x6D46072000
    IPrimitiveComponent.SetOcclusFlag = 0x6D46072000
    IPrimitiveComponent.SetShaderGraphMaterialFlags = 0x6D46072000
    IPrimitiveComponent.SetShaderGraphParameter = 0x6D46072000
    IPrimitiveComponent.SetShaderGraphParameterWithArray = 0x6D46072000
    IPrimitiveComponent.SetShaderMaterialFlags = 0x6D46072000
    IPrimitiveComponent.SetShaderParameter = 0x6D46072000
    IStoryboard.AddCallback = 0x6D46072000
    IStoryboard.AddFloatEasing = 0x6D46072000
    IStoryboard.AddSpeedKey = 0x6D46072000
    IStoryboard.ClearSpeedKeys = 0x6D46072000
    IStoryboard.EnterStoryboard = 0x6D46072000
    IStoryboard.LeaveStoryboard = 0x6D46072000
    IStoryboard.Move = 0x6D46072000
    IStoryboard.SetAutoFrameLimit = 0x6D46072000
    IStoryboard.Step = 0x6D46072000
    IStoryboard.Stop = 0x6D46072000
    IWorld.AddLevel = 0x6D46072000
    IWorld.AddSubNavigateMap = 0x6D46072000
    IWorld.DelLevel = 0x6D46072000
    IWorld.GetWorldBB = 0x6D46072000
    IWorld.LoadLevel = 0x6D46072000
    IWorld.RemoveSubNavigateMap = 0x6D46072000
    IWorld.RenameLevel = 0x6D46072000
    IWorld.SetAlternativeColorGradingMap = 0x6D46072000
    IWorld.SetAnchor = 0x6D46072000
    ImageComponent.Cache = 0x6D46072000
    ImageComponent.Flush = 0x6D46072000
    ImageComponent.GetHeight = 0x6D46072000
    ImageComponent.GetPixel = 0x6D46072000
    ImageComponent.GetWidth = 0x6D46072000
    ImageComponent.Load = 0x6D46072000
    ImageComponent.LoadTexture = 0x6D46072000
    ImageComponent.ResizeImage = 0x6D46072000
    ImageComponent.Save = 0x6D46072000
    ImageComponent.SaveInMemory = 0x6D46072000
    ImageComponent.UpSample2 = 0x6D46072000
    ImageComponent.UpdateRegion = 0x6D46072000
    ModelComponent.ApplyBlendShapeWeight = 0x6D46072000
    ModelComponent.ApplyCustomMaterial = 0x6D46072000
    ModelComponent.ClearCustomLodScreenSize = 0x6D46072000
    ModelComponent.CreateCustomLodScreenSize = 0x6D46072000
    ModelComponent.DisableMipDistance = 0x6D46072000
    ModelComponent.GetBlendShapeActions = 0x6D46072000
    ModelComponent.GetBlendShapeAnimationDuration = 0x6D46072000
    ModelComponent.GetBlendShapeAnimations = 0x6D46072000
    ModelComponent.GetBlendShapeChannels = 0x6D46072000
    ModelComponent.GetCurrentLodScreenSize = 0x6D46072000
    ModelComponent.GetGeometryCacheInfo = 0x6D46072000
    ModelComponent.GetIndices = 0x6D46072000
    ModelComponent.GetMaterialShaderName = 0x6D46072000
    ModelComponent.GetMeshGroupSize = 0x6D46072000
    ModelComponent.GetVertexTraits = 0x6D46072000
    ModelComponent.GetVertices = 0x6D46072000
    ModelComponent.HasBlendShape = 0x6D46072000
    ModelComponent.PlayBlendShapeAnimation = 0x6D46072000
    ModelComponent.ResetBlendShapeWeight = 0x6D46072000
    ModelComponent.ResetCustomMaterial = 0x6D46072000
    ModelComponent.SetBlendShapeActionTime = 0x6D46072000
    ModelComponent.SetCustomLodDistance = 0x6D46072000
    ModelComponent.SetCustomLodScreenSize = 0x6D46072000
    ModelComponent.SetGeometryCacheScale = 0x6D46072000
    ModelComponent.SetGeometryCacheTime = 0x6D46072000
    ModelComponent.SetGeometryCacheVisibility = 0x6D46072000
    ModelComponent.SetLightMapParam = 0x6D46072000
    ModelComponent.SetNoAlpha = 0x6D46072000
    ModelComponent.SetVisibleDistance = 0x6D46072000
    ModelComponent.SetZWrite = 0x6D46072000
    ModelComponent.StopBlendShapeAction = 0x6D46072000
    ParticleComponent.AcquireResource = 0x6D46072000
    ParticleComponent.Activate = 0x6D46072000
    ParticleComponent.AddEvent = 0x6D46072000
    ParticleComponent.AddParameter = 0x6D46072000
    ParticleComponent.ClearParameters = 0x6D46072000
    ParticleComponent.Deactivate = 0x6D46072000
    ParticleComponent.DisablePrinciple = 0x6D46072000
    ParticleComponent.GetCollisionResult = 0x6D46072000
    ParticleComponent.GetParameter = 0x6D46072000
    ParticleComponent.Pause = 0x6D46072000
    ParticleComponent.Seek = 0x6D46072000
    ParticleComponent.SetFinestLODGroup = 0x6D46072000
    ParticleComponent.SetIsPrincipleByScript = 0x6D46072000
    ParticleComponent.SetLODGroup = 0x6D46072000
    ParticleComponent.SetLodDistance = 0x6D46072000
    ParticleComponent.Translate = 0x6D46072000
    ParticleComponent.WillTerminate = 0x6D46072000
    PhysicsCDBodyDebugDrawer.GetDrawerVertices = 0x6D46072000
    PhysicsCDBodyDebugDrawer.GetShapeSpaceDrawerVectices = 0x6D46072000
    PhysicsCollisionAwareSceneBody.LoadByPath = 0x6D46072000
    PhysicsDynamicBody.AddRBReadyCallback = 0x6D46072000
    PhysicsDynamicBody.GetEnableInterpolatePose = 0x6D46072000
    PhysicsDynamicBody.SetEnableInterpolatePose = 0x6D46072000
    PhysicsDynamicBody.SetRBByResPath = 0x6D46072000
    PhysicsDynamicBody.SetRBByWrapper = 0x6D46072000
    PhysicsMaterialWrapper.CreateResource = 0x6D46072000
    PhysicsMaterialWrapper.GetDynamicFriction = 0x6D46072000
    PhysicsMaterialWrapper.GetFrictionCombineType = 0x6D46072000
    PhysicsMaterialWrapper.GetRestitution = 0x6D46072000
    PhysicsMaterialWrapper.GetRestitutionCombineType = 0x6D46072000
    PhysicsMaterialWrapper.GetStaticFriction = 0x6D46072000
    PhysicsMaterialWrapper.IsReady = 0x6D46072000
    PhysicsMaterialWrapper.IsResourceCreated = 0x6D46072000
    PhysicsMaterialWrapper.SetDynamicFriction = 0x6D46072000
    PhysicsMaterialWrapper.SetFrictionCombineType = 0x6D46072000
    PhysicsMaterialWrapper.SetMaterialFromRes = 0x6D46072000
    PhysicsMaterialWrapper.SetRestitution = 0x6D46072000
    PhysicsMaterialWrapper.SetRestitutionCombineType = 0x6D46072000
    PhysicsMaterialWrapper.SetStaticFriction = 0x6D46072000
    PhysicsMaterialWrapper.SetToRuntimeMaterial = 0x6D46072000
    PhysicsRBTemplateWrapper.CreateResource = 0x6D46072000
    PhysicsRBTemplateWrapper.DeepCopy = 0x6D46072000
    PhysicsRBTemplateWrapper.GetAngularDamping = 0x6D46072000
    PhysicsRBTemplateWrapper.GetCenterOfMass = 0x6D46072000
    PhysicsRBTemplateWrapper.GetCollisionFilterInfo = 0x6D46072000
    PhysicsRBTemplateWrapper.GetEnableCCD = 0x6D46072000
    PhysicsRBTemplateWrapper.GetLinearDamping = 0x6D46072000
    PhysicsRBTemplateWrapper.GetMass = 0x6D46072000
    PhysicsRBTemplateWrapper.GetMassLocalPoseRot = 0x6D46072000
    PhysicsRBTemplateWrapper.GetMassSpaceInertiaTensor = 0x6D46072000
    PhysicsRBTemplateWrapper.GetMaterial = 0x6D46072000
    PhysicsRBTemplateWrapper.GetMaterialsCnt = 0x6D46072000
    PhysicsRBTemplateWrapper.GetMaxAngularSpeed = 0x6D46072000
    PhysicsRBTemplateWrapper.GetMaxLinearSpeed = 0x6D46072000
    PhysicsRBTemplateWrapper.GetMotionType = 0x6D46072000
    PhysicsRBTemplateWrapper.GetScale = 0x6D46072000
    PhysicsRBTemplateWrapper.GetShape = 0x6D46072000
    PhysicsRBTemplateWrapper.GetShapeLocalPose = 0x6D46072000
    PhysicsRBTemplateWrapper.GetShapesCnt = 0x6D46072000
    PhysicsRBTemplateWrapper.GetSleepThreshold = 0x6D46072000
    PhysicsRBTemplateWrapper.IsReady = 0x6D46072000
    PhysicsRBTemplateWrapper.IsResourceCreated = 0x6D46072000
    PhysicsRBTemplateWrapper.MergeOtherRBTemplate = 0x6D46072000
    PhysicsRBTemplateWrapper.SetAngularDamping = 0x6D46072000
    PhysicsRBTemplateWrapper.SetCenterOfMass = 0x6D46072000
    PhysicsRBTemplateWrapper.SetCollisionFilterInfo = 0x6D46072000
    PhysicsRBTemplateWrapper.SetEnableCCD = 0x6D46072000
    PhysicsRBTemplateWrapper.SetLinearDamping = 0x6D46072000
    PhysicsRBTemplateWrapper.SetMass = 0x6D46072000
    PhysicsRBTemplateWrapper.SetMassLocalPoseRot = 0x6D46072000
    PhysicsRBTemplateWrapper.SetMassSpaceInertiaTensor = 0x6D46072000
    PhysicsRBTemplateWrapper.SetMaxAngularSpeed = 0x6D46072000
    PhysicsRBTemplateWrapper.SetMaxLinearSpeed = 0x6D46072000
    PhysicsRBTemplateWrapper.SetMotionType = 0x6D46072000
    PhysicsRBTemplateWrapper.SetRBTemplateFromRes = 0x6D46072000
    PhysicsRBTemplateWrapper.SetScale = 0x6D46072000
    PhysicsRBTemplateWrapper.SetShapeLocalPose = 0x6D46072000
    PhysicsRBTemplateWrapper.SetSleepThreshold = 0x6D46072000
    PhysicsRBTemplateWrapper.SetToRuntimeRBTemplate = 0x6D46072000
    PhysicsShapeConfig.GetShapeCollProps = 0x6D46072000
    PhysicsShapeConfig.SetShapeCollProps = 0x6D46072000
    PhysicsShapeInstance.GetIndexInMasterBody = 0x6D46072000
    PhysicsShapeInstance.GetMasterBody = 0x6D46072000
    PhysicsShapeInstance.IsShapeValid = 0x6D46072000
    PhysicsShapeWrapper.GetShapeLocalTransform = 0x6D46072000
    PhysicsShapeWrapper.GetShapeType = 0x6D46072000
    PhysicsShapeWrapper.IsReady = 0x6D46072000
    PhysicsShapeWrapper.SetShapeFromResPath = 0x6D46072000
    PhysicsShapeWrapper.SetShapeLocalTransform = 0x6D46072000
    PhysicsShapeWrapper.SetShapeScale = 0x6D46072000
    PhysicsShapeWrapper.SetShapeToBoxImmediately = 0x6D46072000
    PhysicsShapeWrapper.SetShapeToCapsuleImmediately = 0x6D46072000
    PhysicsShapeWrapper.SetShapeToConvexImmediately = 0x6D46072000
    PhysicsShapeWrapper.SetShapeToSphereImmediately = 0x6D46072000
    PhysicsShapeWrapper.SetShapeToTerrainChunk = 0x6D46072000
    PhysicsStaticBody.AddRBReadyCallback = 0x6D46072000
    PhysicsStaticBody.SetRBByGuid = 0x6D46072000
    PhysicsStaticBody.SetRBByResPath = 0x6D46072000
    PhysicsStaticBody.SetRBByWrapper = 0x6D46072000
    PhysicsTerrainComponent.ShiftOrigin = 0x6D46072000
    RBStateSenderDebugComponent.GetSyncFramePoints = 0x6D46072000
    RBStateSyncerPVB.GetBlendDuration = 0x6D46072000
    RBStateSyncerPVB.GetInterpolateTimeOffset = 0x6D46072000
    RBStateSyncerPVB.SetBlendDuration = 0x6D46072000
    RBStateSyncerPVB.SetInterpolateTimeOffset = 0x6D46072000
    ReflectionProbeComponent.UpdateGuid = 0x6D46072000
    ReflectionProbeComponent.UpdateGuidArray = 0x6D46072000
    SceneComponent.AddEnvironmentVolume = 0x6D46072000
    SceneComponent.AddGlobalVolume = 0x6D46072000
    SceneComponent.AddHeightmap = 0x6D46072000
    SceneComponent.AddLight = 0x6D46072000
    SceneComponent.AddLightProbe = 0x6D46072000
    SceneComponent.AddPSVolume = 0x6D46072000
    SceneComponent.AddPVSCube = 0x6D46072000
    SceneComponent.AddPointCloud = 0x6D46072000
    SceneComponent.AddReflectionProbe = 0x6D46072000
    SceneComponent.AddSHVolume = 0x6D46072000
    SceneComponent.AddSpace = 0x6D46072000
    SceneComponent.AddVisibilityCube = 0x6D46072000
    SceneComponent.CaptureAllReflectionProbe = 0x6D46072000
    SceneComponent.DelEnvironmentVolume = 0x6D46072000
    SceneComponent.DelGlobalVolume = 0x6D46072000
    SceneComponent.DelHeightmap = 0x6D46072000
    SceneComponent.DelLight = 0x6D46072000
    SceneComponent.DelLightProbe = 0x6D46072000
    SceneComponent.DelPSVolume = 0x6D46072000
    SceneComponent.DelPVSCube = 0x6D46072000
    SceneComponent.DelPointCloud = 0x6D46072000
    SceneComponent.DelReflectionProbe = 0x6D46072000
    SceneComponent.DelSHVolume = 0x6D46072000
    SceneComponent.DelSpace = 0x6D46072000
    SceneComponent.DelVisibilityCube = 0x6D46072000
    SceneComponent.GetHeightmapCollection = 0x6D46072000
    SceneComponent.ImportAllReflectionProbe = 0x6D46072000
    SceneComponent.RebuildEnvVolumeTree = 0x6D46072000
    SceneComponent.SetPVSSunShadow = 0x6D46072000
    SceneComponent.TranslateAll = 0x6D46072000
    SceneComponent.UpdateGlobalVolume = 0x6D46072000
    SkeletonComponent.AddBoxCollisionBone = 0x6D46072000
    SkeletonComponent.AddCapsuleCollisionBone = 0x6D46072000
    SkeletonComponent.AddCylinderCollisionBone = 0x6D46072000
    SkeletonComponent.AddExternalSoftBoneCollider = 0x6D46072000
    SkeletonComponent.AddExternalSoftBoneColliderFromSkeleton = 0x6D46072000
    SkeletonComponent.AddSphereCollisionBone = 0x6D46072000
    SkeletonComponent.ClearAllEffects = 0x6D46072000
    SkeletonComponent.ClearCollisionBones = 0x6D46072000
    SkeletonComponent.ClearEffect = 0x6D46072000
    SkeletonComponent.ClearEffectImmediately = 0x6D46072000
    SkeletonComponent.ClearExternalSoftBoneColliders = 0x6D46072000
    SkeletonComponent.ClearHardPoints = 0x6D46072000
    SkeletonComponent.ClearSkeletonEffects = 0x6D46072000
    SkeletonComponent.DelCollisionBone = 0x6D46072000
    SkeletonComponent.DelExternalSoftBoneCollider = 0x6D46072000
    SkeletonComponent.FootskateDetected = 0x6D46072000
    SkeletonComponent.ForwardSoftBoneChains = 0x6D46072000
    SkeletonComponent.GetAllEffectEntities = 0x6D46072000
    SkeletonComponent.GetAllSoftChainProperties = 0x6D46072000
    SkeletonComponent.GetAnimationResource = 0x6D46072000
    SkeletonComponent.GetBipedPoseBones = 0x6D46072000
    SkeletonComponent.GetBoneLocalTransform = 0x6D46072000
    SkeletonComponent.GetBoneRotateRefInit = 0x6D46072000
    SkeletonComponent.GetBoneTransform = 0x6D46072000
    SkeletonComponent.GetBoneTransformBlendingAnimations = 0x6D46072000
    SkeletonComponent.GetBoneTransformInKeyFrames = 0x6D46072000
    SkeletonComponent.GetBoneTransformSafe = 0x6D46072000
    SkeletonComponent.GetBoneVisualTransform = 0x6D46072000
    SkeletonComponent.GetBoneWorldTransform = 0x6D46072000
    SkeletonComponent.GetCollisionBoneCount = 0x6D46072000
    SkeletonComponent.GetDynamicBox = 0x6D46072000
    SkeletonComponent.GetDynamicBoxInWorld = 0x6D46072000
    SkeletonComponent.GetEffectEntities = 0x6D46072000
    SkeletonComponent.GetEffectPlayFinestLod = 0x6D46072000
    SkeletonComponent.GetEffectPlayLod = 0x6D46072000
    SkeletonComponent.GetEffectPlaySpeed = 0x6D46072000
    SkeletonComponent.GetFootstepInfo = 0x6D46072000
    SkeletonComponent.GetParentBone = 0x6D46072000
    SkeletonComponent.GetPlayingEffects = 0x6D46072000
    SkeletonComponent.GetPoseBoneCount = 0x6D46072000
    SkeletonComponent.GetPoseBones = 0x6D46072000
    SkeletonComponent.GetSkeletonEffects = 0x6D46072000
    SkeletonComponent.GetSkeletonResource = 0x6D46072000
    SkeletonComponent.GetSoftBoneLod = 0x6D46072000
    SkeletonComponent.GetSoftBoneTeleportTime = 0x6D46072000
    SkeletonComponent.GetSoftChainNeighbor = 0x6D46072000
    SkeletonComponent.GetSoftChainProperty = 0x6D46072000
    SkeletonComponent.GetSoundBias = 0x6D46072000
    SkeletonComponent.GetSoundRange = 0x6D46072000
    SkeletonComponent.GetSoundVolume = 0x6D46072000
    SkeletonComponent.GetStaticBox = 0x6D46072000
    SkeletonComponent.GetStaticBoxInWorld = 0x6D46072000
    SkeletonComponent.GetVisibilityBox = 0x6D46072000
    SkeletonComponent.HasAnimation = 0x6D46072000
    SkeletonComponent.HasBone = 0x6D46072000
    SkeletonComponent.HasSkinnedBone = 0x6D46072000
    SkeletonComponent.IsEnableBoneConstraint = 0x6D46072000
    SkeletonComponent.IsEnableEffectPlay = 0x6D46072000
    SkeletonComponent.IsEnablePoseCache = 0x6D46072000
    SkeletonComponent.IsEnablePoseSpaceDeformer = 0x6D46072000
    SkeletonComponent.IsEnableScreenEffect = 0x6D46072000
    SkeletonComponent.IsEnableSoftBone = 0x6D46072000
    SkeletonComponent.IsEnableSoftBoneCollider = 0x6D46072000
    SkeletonComponent.IsEnableSoftBoneSmooth = 0x6D46072000
    SkeletonComponent.IsEnableSoftChainByName = 0x6D46072000
    SkeletonComponent.IsEnableSoundPlay = 0x6D46072000
    SkeletonComponent.IsInsureEffectPlay = 0x6D46072000
    SkeletonComponent.IsReadyToAppear = 0x6D46072000
    SkeletonComponent.IsResourceReady = 0x6D46072000
    SkeletonComponent.LoadSkeleton = 0x6D46072000
    SkeletonComponent.LoadSkeletonDeferred = 0x6D46072000
    SkeletonComponent.LoadSkeletonFromMem = 0x6D46072000
    SkeletonComponent.MarkResourceReady = 0x6D46072000
    SkeletonComponent.OverrideEffectParam = 0x6D46072000
    SkeletonComponent.PauseEffect = 0x6D46072000
    SkeletonComponent.PlayAction = 0x6D46072000
    SkeletonComponent.PlayActionWithStartTime = 0x6D46072000
    SkeletonComponent.PlayEffect = 0x6D46072000
    SkeletonComponent.PlaySkeletonEffects = 0x6D46072000
    SkeletonComponent.QueryAnimationCues = 0x6D46072000
    SkeletonComponent.QueryBoneConstraintParams = 0x6D46072000
    SkeletonComponent.QueryCollisionBone = 0x6D46072000
    SkeletonComponent.QueryPoseCache = 0x6D46072000
    SkeletonComponent.ResetSoftBoneChains = 0x6D46072000
    SkeletonComponent.ResetTPos = 0x6D46072000
    SkeletonComponent.ScaleCollisionBones = 0x6D46072000
    SkeletonComponent.SetEffectPlayFinestLod = 0x6D46072000
    SkeletonComponent.SetEffectPlayLod = 0x6D46072000
    SkeletonComponent.SetEffectPlaySpeed = 0x6D46072000
    SkeletonComponent.SetEffectVisible = 0x6D46072000
    SkeletonComponent.SetEnableBoneConstraint = 0x6D46072000
    SkeletonComponent.SetEnableEffectPlay = 0x6D46072000
    SkeletonComponent.SetEnablePoseCache = 0x6D46072000
    SkeletonComponent.SetEnablePoseSpaceDeformer = 0x6D46072000
    SkeletonComponent.SetEnableScreenEffect = 0x6D46072000
    SkeletonComponent.SetEnableSoftBone = 0x6D46072000
    SkeletonComponent.SetEnableSoftBoneCollider = 0x6D46072000
    SkeletonComponent.SetEnableSoftBoneSmooth = 0x6D46072000
    SkeletonComponent.SetEnableSoftChainByName = 0x6D46072000
    SkeletonComponent.SetEnableSoundPlay = 0x6D46072000
    SkeletonComponent.SetInsureEffectPlay = 0x6D46072000
    SkeletonComponent.SetPrinciple = 0x6D46072000
    SkeletonComponent.SetSoftBoneLod = 0x6D46072000
    SkeletonComponent.SetSoftBoneTeleportTime = 0x6D46072000
    SkeletonComponent.SetSoftChainNeighbor = 0x6D46072000
    SkeletonComponent.SetSoftChainProperty = 0x6D46072000
    SkeletonComponent.SetSoundBias = 0x6D46072000
    SkeletonComponent.SetSoundRange = 0x6D46072000
    SkeletonComponent.SetSoundVolume = 0x6D46072000
    SkeletonComponent.SetVisibilityBox = 0x6D46072000
    SkeletonComponent.StopAction = 0x6D46072000
    SkeletonComponent.StopActionAt = 0x6D46072000
    SkeletonComponent.SwitchCollisionBones = 0x6D46072000
    SkeletonComponent.SwitchSoftBoneParams = 0x6D46072000
    SkeletonComponent.isPrincipleSound = 0x6D46072000
    SkeletonComponent.setPrincipleSound = 0x6D46072000
    SnowTrailComponent.SetTrailProperty = 0x6D46072000
    SoundComponent.AddDSP = 0x6D46072000
    SoundComponent.AddMediaPath = 0x6D46072000
    SoundComponent.ChangeAudioSessionProperties = 0x6D46072000
    SoundComponent.ClearEventData = 0x6D46072000
    SoundComponent.ConvertAmrToWavAsync = 0x6D46072000
    SoundComponent.ConvertWavToAmrAsync = 0x6D46072000
    SoundComponent.DecGameObjectRefCount = 0x6D46072000
    SoundComponent.DecMediaRefCount = 0x6D46072000
    SoundComponent.EnableFirstThirdPersonDistinguish = 0x6D46072000
    SoundComponent.EnableNonActivePlaying = 0x6D46072000
    SoundComponent.EndNativeRecording = 0x6D46072000
    SoundComponent.EndRecording = 0x6D46072000
    SoundComponent.EndRecordingWithPath = 0x6D46072000
    SoundComponent.ExecuteActionOnEvent = 0x6D46072000
    SoundComponent.GameObjectStopAll = 0x6D46072000
    SoundComponent.GenerateAndRegisterGameObject = 0x6D46072000
    SoundComponent.GetActiveEventCount = 0x6D46072000
    SoundComponent.GetCurrentMediaList = 0x6D46072000
    SoundComponent.GetEventLength = 0x6D46072000
    SoundComponent.GetEventParameter = 0x6D46072000
    SoundComponent.GetEventVolume = 0x6D46072000
    SoundComponent.GetGameObjectID = 0x6D46072000
    SoundComponent.GetGameObjectIDWithSwitches = 0x6D46072000
    SoundComponent.GetGlobalParameter = 0x6D46072000
    SoundComponent.GetInputDevices = 0x6D46072000
    SoundComponent.GetLength = 0x6D46072000
    SoundComponent.GetOutputDevices = 0x6D46072000
    SoundComponent.GetPosition = 0x6D46072000
    SoundComponent.GetSoundAPI = 0x6D46072000
    SoundComponent.GetSpeakerMode = 0x6D46072000
    SoundComponent.HandleMusicCue = 0x6D46072000
    SoundComponent.IncGameObjectRefCount = 0x6D46072000
    SoundComponent.IncMediaRefCount = 0x6D46072000
    SoundComponent.IsNativeRecording = 0x6D46072000
    SoundComponent.IsPlaying = 0x6D46072000
    SoundComponent.KeyOffEvent = 0x6D46072000
    SoundComponent.LoadMedia = 0x6D46072000
    SoundComponent.LoadMediaAsyncConsole = 0x6D46072000
    SoundComponent.OnActivated = 0x6D46072000
    SoundComponent.PauseSound = 0x6D46072000
    SoundComponent.Play3DEvent = 0x6D46072000
    SoundComponent.Play3DEvent2 = 0x6D46072000
    SoundComponent.Play3DEventFollowTarget = 0x6D46072000
    SoundComponent.Play3DEventFollowTarget2 = 0x6D46072000
    SoundComponent.Play3DSoundCategoryExt = 0x6D46072000
    SoundComponent.Play3DSoundExt = 0x6D46072000
    SoundComponent.Play3DSoundFollowTargetCategoryExt = 0x6D46072000
    SoundComponent.Play3DSoundFollowTargetExt = 0x6D46072000
    SoundComponent.PlayAmrSound = 0x6D46072000
    SoundComponent.PlayEvent = 0x6D46072000
    SoundComponent.PlayEvent2 = 0x6D46072000
    SoundComponent.PlayNativeRecording = 0x6D46072000
    SoundComponent.PlayRecording = 0x6D46072000
    SoundComponent.PlaySound = 0x6D46072000
    SoundComponent.PlaySoundCategoryExt = 0x6D46072000
    SoundComponent.PlaySoundExt = 0x6D46072000
    SoundComponent.PostEvent = 0x6D46072000
    SoundComponent.PostEventWithoutMedia = 0x6D46072000
    SoundComponent.PostMIDIOnEvent = 0x6D46072000
    SoundComponent.PostRougeEvent = 0x6D46072000
    SoundComponent.PrintEventInfo = 0x6D46072000
    SoundComponent.QueryRoom = 0x6D46072000
    SoundComponent.ReleaseMusicCue = 0x6D46072000
    SoundComponent.RemoveAllSounds = 0x6D46072000
    SoundComponent.RemoveDSP = 0x6D46072000
    SoundComponent.RemoveEventParameter = 0x6D46072000
    SoundComponent.RemoveGlobalParameter = 0x6D46072000
    SoundComponent.SeekOnEvent = 0x6D46072000
    SoundComponent.Set3DSoundMinMaxDistance = 0x6D46072000
    SoundComponent.Set3DSoundPosition = 0x6D46072000
    SoundComponent.SetAmrVolume = 0x6D46072000
    SoundComponent.SetDSPParameter = 0x6D46072000
    SoundComponent.SetEvent3DMinMaxDistance = 0x6D46072000
    SoundComponent.SetEventFadeout = 0x6D46072000
    SoundComponent.SetEventMute = 0x6D46072000
    SoundComponent.SetEventParameter = 0x6D46072000
    SoundComponent.SetEventPaused = 0x6D46072000
    SoundComponent.SetEventPosition = 0x6D46072000
    SoundComponent.SetEventPositionAndOrientation = 0x6D46072000
    SoundComponent.SetEventPositionAndOrientationBias = 0x6D46072000
    SoundComponent.SetEventVolume = 0x6D46072000
    SoundComponent.SetEventVolumeRamp = 0x6D46072000
    SoundComponent.SetGameObjectMultiplePositions = 0x6D46072000
    SoundComponent.SetGameObjectRTPCValue = 0x6D46072000
    SoundComponent.SetGameObjectScalingFactor = 0x6D46072000
    SoundComponent.SetGameObjectSwitch = 0x6D46072000
    SoundComponent.SetGameSilent = 0x6D46072000
    SoundComponent.SetGlobalParameter = 0x6D46072000
    SoundComponent.SetGlobalReverb = 0x6D46072000
    SoundComponent.SetLoadMediaCallback = 0x6D46072000
    SoundComponent.SetMaxRecordTime = 0x6D46072000
    SoundComponent.SetMediaPath = 0x6D46072000
    SoundComponent.SetMusicParameterValue = 0x6D46072000
    SoundComponent.SetNativeDeviceID = 0x6D46072000
    SoundComponent.SetNextGameObjectID = 0x6D46072000
    SoundComponent.SetNextGameObjectName = 0x6D46072000
    SoundComponent.SetPlayAmrCallback = 0x6D46072000
    SoundComponent.SetPlayAmrCallbackWithPath = 0x6D46072000
    SoundComponent.SetPosition = 0x6D46072000
    SoundComponent.SetRecordSampleRate = 0x6D46072000
    SoundComponent.SetSoundVolume = 0x6D46072000
    SoundComponent.SetSpeakerMode = 0x6D46072000
    SoundComponent.SetUnloadEventCallback = 0x6D46072000
    SoundComponent.SetVolumeScale = 0x6D46072000
    SoundComponent.StartNativeRecording = 0x6D46072000
    SoundComponent.StartRecording = 0x6D46072000
    SoundComponent.StopAmrSound = 0x6D46072000
    SoundComponent.StopMIDIOnEvent = 0x6D46072000
    SoundComponent.StopPlayingNativeRecording = 0x6D46072000
    SoundComponent.StopPlayingRecording = 0x6D46072000
    SoundComponent.StopSound = 0x6D46072000
    SoundComponent.TranslateAll = 0x6D46072000
    SoundComponent.UnRegisterGameObject = 0x6D46072000
    SoundComponent.UnloadEvent = 0x6D46072000
    SoundComponent.UnloadEvent2 = 0x6D46072000
    SoundComponent.UnloadMedia = 0x6D46072000
    SoundComponent.UnloadMediaFile = 0x6D46072000
    SoundComponent.UnloadMediaFileAsyncConsole = 0x6D46072000
    SpaceComponent.AddLoadable = 0x6D46072000
    SpaceComponent.AddPrimitive = 0x6D46072000
    SpaceComponent.DelLoadable = 0x6D46072000
    SpaceComponent.DelPrimitive = 0x6D46072000
    SpaceComponent.UpdateLoadable = 0x6D46072000
    SpaceComponent.UpdatePrimitive = 0x6D46072000
    TMatrix4x3<float> (Messiah.IObject.*)(const std.string &) noexcept>.__name() [_type = Messiah.TMatrix4x3<float> (Messiah.IObject.*)(const std.string &) noexcept] = 0x10
    TachComponent.Attach = 0x6D46072000
    TachComponent.Detach = 0x6D46072000
    TerrainComponent.AddSphere = 0x6D46072000
    TerrainComponent.BuildChunkToRigidBodyMap = 0x6D46072000
    TerrainComponent.EditClear = 0x6D46072000
    TerrainComponent.EditColor = 0x6D46072000
    TerrainComponent.EditShape = 0x6D46072000
    TerrainComponent.GetAuxiliaryMap = 0x6D46072000
    TerrainComponent.GetBlockIndex = 0x6D46072000
    TerrainComponent.GetBlockTag = 0x6D46072000
    TerrainComponent.GetChunkOffset = 0x6D46072000
    TerrainComponent.GetCoveredChunks = 0x6D46072000
    TerrainComponent.LoadHeightField = 0x6D46072000
    TerrainComponent.LoadHeightFieldOfChunk = 0x6D46072000
    TerrainComponent.LoadHeightFieldRaw = 0x6D46072000
    TerrainComponent.RemoveSphere = 0x6D46072000
    TerrainComponent.SaveHeightField = 0x6D46072000
    TerrainComponent.SaveHeightFieldOfChunk = 0x6D46072000
    TerrainComponent.SetAuxiliaryMap = 0x6D46072000
    VehicleCompStateSyncerInterpolation.GetSimSyncBlendParams = 0x6D46072000
    VehicleCompStateSyncerInterpolation.SetSimSyncBlendParams = 0x6D46072000
    VehicleCompStateSyncerPVB.GetBlendDuration = 0x6D46072000
    VehicleCompStateSyncerPVB.GetInterpolateTimeOffset = 0x6D46072000
    VehicleCompStateSyncerPVB.SetBlendDuration = 0x6D46072000
    VehicleCompStateSyncerPVB.SetInterpolateTimeOffset = 0x6D46072000
    VehicleComponent.ConfigVehicle = 0x6D46072000
    VehicleComponent.EnableAutoBox = 0x6D46072000
    VehicleComponent.GearDown = 0x6D46072000
    VehicleComponent.GearTo = 0x6D46072000
    VehicleComponent.GearUp = 0x6D46072000
    VehicleComponent.GetCollisionModifyData = 0x6D46072000
    VehicleComponent.GetCurrentGear = 0x6D46072000
    VehicleComponent.GetEnableSuspsionSweep = 0x6D46072000
    VehicleComponent.GetIsShapeUseCollPropsAlone = 0x6D46072000
    VehicleComponent.GetShapeCollProps = 0x6D46072000
    VehicleComponent.GetVehicleSimParamBool = 0x6D46072000
    VehicleComponent.GetVehicleSimParamFloat = 0x6D46072000
    VehicleComponent.GetVehicleSimParamInt = 0x6D46072000
    VehicleComponent.GetVehicleSimParamVec3 = 0x6D46072000
    VehicleComponent.IsAutoBox = 0x6D46072000
    VehicleComponent.ListAllVehicleSimParam = 0x6D46072000
    VehicleComponent.Reset = 0x6D46072000
    VehicleComponent.SetCollisionModifyData = 0x6D46072000
    VehicleComponent.SetEnableSuspsionSweep = 0x6D46072000
    VehicleComponent.SetIsShapeUseCollPropsAlone = 0x6D46072000
    VehicleComponent.SetPermanentForce = 0x6D46072000
    VehicleComponent.SetPermanentTorque = 0x6D46072000
    VehicleComponent.SetShapeCollProps = 0x6D46072000
    VehicleComponent.SetVehicleSimParamBool = 0x6D46072000
    VehicleComponent.SetVehicleSimParamFloat = 0x6D46072000
    VehicleComponent.SetVehicleSimParamInt = 0x6D46072000
    VehicleComponent.SetVehicleSimParamVec3 = 0x6D46072000
    VehicleConfig.ConfigAllTerrainVehicle = 0x6D46072000
    VehicleConfig.ConfigFakeMotorCycleSideCarVehicle = 0x6D46072000
    VehicleConfig.ConfigHanmaVehicle = 0x6D46072000
    VehicleConfig.ConfigJeep2Vehicle = 0x6D46072000
    VehicleConfig.ConfigJeepVehicle = 0x6D46072000
    VehicleConfig.ConfigMotorBikeVehicle = 0x6D46072000
    VehicleConfig.ConfigMotorCycleSideCarVehicle = 0x6D46072000
    VehicleConfig.GetChassisMaterials = 0x6D46072000
    VehicleConfig.GetSingleConvexHull = 0x6D46072000
    VehicleConfig.GetVehicleShapes = 0x6D46072000
    VehicleConfig.RecalculateSprungMass = 0x6D46072000
    VehicleSurfaceToTireFrictionPairsConfig.AddPair = 0x6D46072000
    VehicleSurfaceToTireFrictionPairsConfig.Clear = 0x6D46072000
    VehicleSystemComponent.GetCanRideOn = 0x6D46072000
    VehicleSystemComponent.GetChassisRB = 0x6D46072000
    VehicleSystemComponent.GetCollisionFilterData = 0x6D46072000
    VehicleSystemComponent.GetVehicleSimParamBool = 0x6D46072000
    VehicleSystemComponent.GetVehicleSimParamDimension = 0x6D46072000
    VehicleSystemComponent.GetVehicleSimParamFloat = 0x6D46072000
    VehicleSystemComponent.GetVehicleSimParamInt = 0x6D46072000
    VehicleSystemComponent.GetVehicleSimParamVec3 = 0x6D46072000
    VehicleSystemComponent.IsInAir = 0x6D46072000
    VehicleSystemComponent.ListAllVehicleSimParam = 0x6D46072000
    VehicleSystemComponent.Reset = 0x6D46072000
    VehicleSystemComponent.SetCanRideOn = 0x6D46072000
    VehicleSystemComponent.SetCollisionFilterData = 0x6D46072000
    VehicleSystemComponent.SetDebugOutput = 0x6D46072000
    VehicleSystemComponent.SetTemplate = 0x6D46072000
    VehicleSystemComponent.SetVehicleSimParamBool = 0x6D46072000
    VehicleSystemComponent.SetVehicleSimParamFloat = 0x6D46072000
    VehicleSystemComponent.SetVehicleSimParamInt = 0x6D46072000
    VehicleSystemComponent.SetVehicleSimParamVec3 = 0x6D46072000
    VehicleSystemTemplateWrapper.CopyFromOther = 0x6D46072000
    VehicleSystemTemplateWrapper.ResetConstraints = 0x6D46072000
    VehicleSystemTemplateWrapper.ResetRBs = 0x6D46072000
    VehicleSystemTemplateWrapper.ResetRBsWithName = 0x6D46072000
    VehicleSystemTemplateWrapper.SetToVehicleUGC = 0x6D46072000
    VirtualCamera.Free = 0x6D46072000
    VirtualCameraManager.AddController = 0x6D46072000
    VirtualCameraManager.Apply = 0x6D46072000
    VirtualCameraManager.Clear = 0x6D46072000
    VirtualCameraManager.DelController = 0x6D46072000
    VirtualCameraManager.GetCameraPropertyByString = 0x6D46072000
    VirtualCameraManager.Insert = 0x6D46072000
    VirtualCameraManager.QueryCameraProperties = 0x6D46072000
    VirtualCameraManager.Remove = 0x6D46072000
    VirtualCameraManager.Resolve = 0x6D46072000
    VirtualCameraManager.SetCameraPropertyByString = 0x6D46072000
    WaterWaveComponent.GenerateCullingFlags = 0x6D46072000
    WaterWaveComponent.ReconstructLodMesh = 0x6D46072000
    WaterWaveComponent.ResetCullingFlags = 0x6D46072000
    WaterWaveComponent.SetWaterWaveMaterial = 0x6D46072000
    WaterWaveComponent.SetWaterWaveResource = 0x6D46072000

class ModelComponent:
    BatchKey = 0x3D0
    Distance = 0x2F4
    EnvMap = 0x3A0
    HideInMainView = 0x3D5
    InScreenDepthMask = 0x3D4
    IsDeformable = 0x3CD
    IsEnvMapReady = 0x361
    IsHiddenUntilReady = 0x3F8
    IsLightmapConst = 0x3CC
    IsLightmapReady = 0x360
    Lightmap = 0x368
    LightmapAlwaysLoad = 0x398
    LightmapOffset = 0x388
    LightmapScale = 0x378
    LodFadingTime = 0x288
    NoFarPlaneClip = 0x362
    ReceiveDecals = 0x363
    ReflectionBox = 0x3B0
    ReflectionProbeLum = 0x3C8
    SkinCacheMode = 0x3D6
    UseCustomMaterialOnly = 0x3CE

class ModelInfo:
    ModelElements = 0x24
    ModelType = 0x20
    NumFaces = 0x1C
    NumVertices = 0x18

class ModelLodInfo:
    BoundingVolume = 0x1C
    LodDistance = 0x38
    LodScreenSize = 0x50
    ModelFlags = 0x80
    ModelGuids = 0x68

class ModifierBlender:
    BlendTime = 0x98
    Direction = 0xB0
    EnableModifier = 0xA3
    Finished = 0xA0
    Fov = 0xBC
    InvalidLastInfo = 0xC0
    TargetPos = 0xA4

class Module contains faster C implementation of abc:
    ABCMeta = 0x20

class MotorBikeBalanceData:
    BalanceFailedThreshold = 0xC
    BalanceRecoverTime = 0x10
    EnableControll = 0x18
    RollScale = 0x14

class MotorBikeBalanceParams:
    BalanceFailedThreshold = 0xC
    BalanceRecoverTime = 0x10
    EnableControll = 0x18
    RollScale = 0x14

class MotorBikeBalanceState:
    IsActivated = 0xC

class NaviObShapeBox:
    Height = 0x10
    Length = 0x14
    Width = 0xC

class NaviObShapeCylinder:
    Height = 0x10
    Radius = 0xC
    Subdiv = 0x14

class NaviObShapeDebugDrawer:
    Show = 0x60

class NaviRecast:
    RecastMapAutoLoader.AddAuxFocusArea = 0x6D46072000
    RecastMapAutoLoader.ClearAllAuxFocusArea = 0x6D46072000
    RecastMapAutoLoader.Finalize = 0x6D46072000
    RecastMapAutoLoader.GetFocusAreaCenters = 0x6D46072000
    RecastMapAutoLoader.Initial = 0x6D46072000
    RecastMapAutoLoader.IsInitialized = 0x6D46072000
    RecastMapDisplayerComponent.EnterSpace = 0x6D46072000
    RecastMapDisplayerComponent.LeaveSpace = 0x6D46072000
    RecastMapDisplayerComponent.Refresh = 0x6D46072000
    RecastMapDisplayerComponent.SetMapEntityTransfrom = 0x6D46072000

class Navigate:
    INavigateGadget.AddVertex = 0x6D46072000
    INavigateGadget.ClearVertices = 0x6D46072000
    INavigateGadget.GetVerticesCount = 0x6D46072000
    INavigateMap.CanAddDynamicObstacle = 0x6D46072000
    INavigateMap.EnableProfileDynObs = 0x6D46072000
    INavigateMap.FindOptPath = 0x6D46072000
    INavigateMap.FindPath = 0x6D46072000
    INavigateMap.FindPathF = 0x6D46072000
    INavigateMap.FindPathFExt = 0x6D46072000
    INavigateMap.GetAllHeight = 0x6D46072000
    INavigateMap.GetAllHeightF = 0x6D46072000
    INavigateMap.GetCAFixRef = 0x6D46072000
    INavigateMap.GetHeight = 0x6D46072000
    INavigateMap.GetHeightF = 0x6D46072000
    INavigateMap.GetHeightInMapFrame = 0x6D46072000
    INavigateMap.GetHeightInMapFrameF = 0x6D46072000
    INavigateMap.GetHeightInMapFrameR = 0x6D46072000
    INavigateMap.GetHeightR = 0x6D46072000
    INavigateMap.GetMapBlockSize = 0x6D46072000
    INavigateMap.GetMapMask = 0x6D46072000
    INavigateMap.GetMapMaskF = 0x6D46072000
    INavigateMap.GetMapMaskR = 0x6D46072000
    INavigateMap.GetProfileDynObsTimes = 0x6D46072000
    INavigateMap.GetSuggestPoint = 0x6D46072000
    INavigateMap.GetSuggestPointF = 0x6D46072000
    INavigateMap.GetSuggestPointR = 0x6D46072000
    INavigateMap.GetTilePos2D = 0x6D46072000
    INavigateMap.GetTileSpan = 0x6D46072000
    INavigateMap.IsCAMapConnectionMarked = 0x6D46072000
    INavigateMap.IsConnected = 0x6D46072000
    INavigateMap.IsConnectedCAMap = 0x6D46072000
    INavigateMap.IsConnectedF = 0x6D46072000
    INavigateMap.IsConnectedR = 0x6D46072000
    INavigateMap.IsDynamicObstacleRequestFull = 0x6D46072000
    INavigateMap.IsObstaclePoolFull = 0x6D46072000
    INavigateMap.IsSupportCAMap = 0x6D46072000
    INavigateMap.IsSupportDynamicObstacle = 0x6D46072000
    INavigateMap.IsTileLoaded = 0x6D46072000
    INavigateMap.LoadMapBlockFromMap = 0x6D46072000
    INavigateMap.LoadMapBlockFromMapAndUpdateCAMap = 0x6D46072000
    INavigateMap.LoadMapFragmentAt = 0x6D46072000
    INavigateMap.MapToWorldPos = 0x6D46072000
    INavigateMap.MarkCAMapConnection = 0x6D46072000
    INavigateMap.QueryNearestPos = 0x6D46072000
    INavigateMap.QueryNearestPosF = 0x6D46072000
    INavigateMap.Raycast = 0x6D46072000
    INavigateMap.RaycastF = 0x6D46072000
    INavigateMap.RaycastFExt = 0x6D46072000
    INavigateMap.RaycastR = 0x6D46072000
    INavigateMap.ShiftOrigin = 0x6D46072000
    INavigateMap.TileToUpdated = 0x6D46072000
    INavigateMap.UnloadMapBlock = 0x6D46072000
    INavigateMap.UnloadMapBlockAndUpdateCAMap = 0x6D46072000
    INavigateMap.UnloadMapFragmentAt = 0x6D46072000
    INavigateMap.WorldToMapPos = 0x6D46072000
    INavigateObstacle.CreateWithShape = 0x6D46072000
    INavigateObstacle.EnterNaviMap = 0x6D46072000
    INavigateObstacle.LeaveNaviMap = 0x6D46072000
    INavigateObstacle.ShiftOrigin = 0x6D46072000
    INavigateShapeWrapper.SetShapeToBoxImmediately = 0x6D46072000
    INavigateShapeWrapper.SetShapeToCylinderImmediately = 0x6D46072000
    INavigateShapeWrapper.SetShapeToCylinderWithSubdivImmediately = 0x6D46072000
    INavigateTrigger.AddVertex = 0x6D46072000
    INavigateTrigger.ClearVertices = 0x6D46072000
    INavigateTrigger.GetVerticesCount = 0x6D46072000
    INavigatorComponent.Cancel = 0x6D46072000
    INavigatorComponent.EnterNaviMap = 0x6D46072000
    INavigatorComponent.FollowTarget = 0x6D46072000
    INavigatorComponent.FollowTargetWithOffset = 0x6D46072000
    INavigatorComponent.GetCurrentWayPoints = 0x6D46072000
    INavigatorComponent.GetPlatformEntity = 0x6D46072000
    INavigatorComponent.GetWayPoints = 0x6D46072000
    INavigatorComponent.LeaveNaviMap = 0x6D46072000
    INavigatorComponent.MoveTo = 0x6D46072000
    INavigatorComponent.MoveToExt = 0x6D46072000
    INavigatorComponent.MoveToWithRadius = 0x6D46072000
    INavigatorComponent.SetFlying = 0x6D46072000
    INavigatorComponent.SetOffmeshAnimMaxTime = 0x6D46072000
    INavigatorComponent.SetPlatformEntity = 0x6D46072000
    INavigatorComponent.SetPosAndYawInWorldFrame = 0x6D46072000
    INavigatorComponent.SetTarget = 0x6D46072000
    INavigatorComponent.SetTargetWithOffset = 0x6D46072000

class NoiseController:
    CurrentPosition = 0xE4
    FinalPitch = 0x120
    FinalPosition = 0x114
    FinalRoll = 0x128
    FinalYaw = 0x124
    PositionNoise = 0x90
    PositionNoiseFPS = 0xA8
    PositionNoiseWeight = 0xAC
    RotationNoise = 0xB8
    RotationNoiseFPS = 0xD0
    RotationNoiseWeight = 0xD4

class OpenSSL X9:
    42 DH method = 0x28

class Operator interface:
    This module exports a set of functions implemented in C corresponding
to the intrinsic operators of Python.  For example, operator.add(x, y)
is equivalent to the expression x+y.  The function names are those
used for special methods; variants without leading and trailing
'__' are also provided for convenience. = 0x10

class OrbitPlacer:
    Cursor = 0xA0
    Direction = 0x11C
    Finished = 0x105
    Fov = 0x108
    FovEnabled = 0x106
    Loop = 0x104
    OrbitDuration = 0xA4
    OrbitResourcePath = 0x88
    Origin = 0xF8
    Roll = 0x10C
    RotateYaw = 0xF4
    StartTime = 0xE4
    Suspend = 0xA8
    SuspendCursor = 0xAC
    Target = 0x150
    TargetPos = 0x110
    TargetTransform = 0x158
    TotalTime = 0xE0
    YawPivot = 0xE8

class OrderInfo:
    Currency = 0x60
    Desc = 0x30
    Etc = 0x48
    OrderId = 0x18

class OverrideMaterialInfo:
    ShaderName = 0x10

class PSVolumeComponent:
    Index = 0xC0
    Resource = 0xB0
    WorldBB = 0xC8

class PSVolumeSpaceComponent:
    PSVolumes = 0xB8
    TileHeight = 0x94
    TileSize = 0x90
    WorldBB = 0x98
    XSize = 0xB0
    ZSize = 0xB4

class PVSCubeComponent:
    Index = 0xF8
    Resource = 0xE8
    WorldBB = 0x100

class PVSSpaceComponent:
    CameraHeight = 0x9C
    PVSCubes = 0xD0
    PVSSunShadow = 0xE8
    TileHeight0 = 0xA0
    TileHeight1 = 0xA4
    TileHeight2 = 0xA8
    TileSize0 = 0x90
    TileSize1 = 0x94
    TileSize2 = 0x98
    WorldBB = 0xAC
    XSize = 0xC4
    ZSize = 0xC8

class PVSSunShadowComponent:
    Resource = 0xC8

class ParameterCollectionInfo:
    isDynamic = 0x30
    vectors = 0x18

class ParameterCollectionVectorNode:
    ParameterName = 0x10

class ParticleAttachBoneInfo:
    Offset = 0x1C
    Velocity = 0x28
    Weight = 0x18

class ParticleAudioEventSlot:
    DelayTime = 0x18
    FadeOutTime = 0x1C
    FadeOutType = 0x20

class ParticleAudioSwitchSlot:
    Value = 0x18

class ParticleComponent:
    AudioSwitches = 0x3A0
    CameraAttaching = 0x2FC
    DeferredTick = 0x312
    HoldResource = 0x311
    InstanceColor = 0x2EC
    IsActive = 0x30E
    IsPaused = 0x30F
    IsStopped = 0x310
    LODGroup = 0x304
    LodDistance = 0x308
    MiscFlags = 0x313
    ParticleParameters = 0x2C0
    RelativeVolume = 0x300
    SleepPeriod = 0x2E8

class ParticleElement:
    Enable = 0x40
    Supported = 0x41

class ParticleElementAlphaByLife:
    AlphaCurve = 0x50
    CurveGuid = 0x68

class ParticleElementAttachBone:
    AffectVelocity = 0x81
    Bones = 0x88
    EntityParameter = 0x50
    ModelEmitter = 0x68
    UniformDistributed = 0x80

class ParticleElementAttachMesh:
    FollowParent = 0x8C
    MeshPath = 0x50
    PrimitiveIndex = 0x8D
    Rotation = 0x74
    Scale = 0x68
    Translation = 0x80

class ParticleElementBladeSource:
    BladeDelay = 0x58
    BladeDuration = 0x54
    BladeInterval = 0x50

class ParticleElementBurstSource:
    CurveGuid = 0x70
    EmissionBursts = 0x88
    EmissionCurve = 0x58
    EmissionDuration = 0x54
    EmissionRate = 0x50
    LoopPeriod = 0xA0

class ParticleElementCollision:
    CollideDistance = 0x58
    Elasticity = 0x54
    FilterInfo = 0x50
    KillVelocity = 0x5C
    RecordResult = 0x61
    SimpleCollision = 0x60

class ParticleElementColorByLife:
    ColorCurve = 0x50
    CoupledMode = 0x80
    CurveGuid = 0x68

class ParticleElementColorByTime:
    ColorCurve = 0x58
    CurveGuid = 0x70
    Loop = 0x54
    OverLife = 0x55
    Peroid = 0x50

class ParticleElementControlByLife:
    ControlCurve = 0x50
    CurveGuid = 0x68

class ParticleElementDynamics:
    Attractor = 0x60
    AttractorAttenuation = 0x7C
    AttractorScale = 0x6C
    AttractorStrength = 0x78
    DragCoefficient = 0x5C
    Gravity = 0x50

class ParticleElementEmissionByLife:
    CurveGuid = 0x68
    EmissionCurve = 0x50

class ParticleElementEvent:
    EventName = 0x50
    OnBirth = 0x58
    OnCollision = 0x5A
    OnDeath = 0x59

class ParticleElementFreeze:
    FreezeLife = 0x50
    TargetLife = 0x54

class ParticleElementGhost:
    FollowParent = 0x50
    PrimitiveIndex = 0x51
    PrimitiveName = 0x58
    SourceEmitter = 0x70

class ParticleElementImpact:
    FilterInfo = 0x50
    MaxDistanceDivergence = 0x5C
    MaxNormalDivergence = 0x60
    PrimaryRayDistance = 0x54
    SecondaryRayDistance = 0x58
    SimulateGravity = 0x64
    UncollidedKillLife = 0x68

class ParticleElementInheritVelocity:
    FollowAcceleration = 0x68
    FollowMode = 0x64
    InheritDuration = 0x5C
    InheritScale = 0x50
    MaxVelocity = 0x60

class ParticleElementInitColor:
    ColorDistribution = 0x50
    ColorParameter = 0x58

class ParticleElementInitControl:
    ControlDistribution = 0x50

class ParticleElementInitEmission:
    EmissionDistribution = 0x50

class ParticleElementInitLife:
    LifeDistribution = 0x50
    LifeParameter = 0x58

class ParticleElementInitLifeByTime:
    CurveGuid = 0x70
    InitLifeCurve = 0x58
    Loop = 0x54
    Peroid = 0x50

class ParticleElementInitPosition:
    PositionDistribution = 0x50
    PositionParameter = 0x58

class ParticleElementInitRotation:
    RotationDistribution = 0x50

class ParticleElementInitRotationRate:
    RotationRateDistribution = 0x50

class ParticleElementInitSize:
    SizeDistribution = 0x50

class ParticleElementInitSize2D:
    SizeDistributionX = 0x50
    SizeDistributionY = 0x58

class ParticleElementInitSize3D:
    Constraint = 0x50
    SizeDistributionX = 0x58
    SizeDistributionY = 0x60
    SizeDistributionZ = 0x68

class ParticleElementInitSpin:
    SpinDistribution = 0x50

class ParticleElementInitSpinRate:
    SpinRateDistribution = 0x50

class ParticleElementInitSubUV:
    TileIndexMax = 0x51
    TileIndexMin = 0x50

class ParticleElementInitVelocity:
    VelocityDistribution = 0x50
    VelocityParameter = 0x58

class ParticleElementMotion:
    Animation = 0x68
    AutoClear = 0xBB
    AutoReverse = 0xB9
    AutoStop = 0xBA
    BoneName = 0x80
    CurveSpeed = 0xA0
    LocalMotion = 0xBC
    Loop = 0xB8
    SkeletonPath = 0x50
    Speed = 0x98

class ParticleElementMovingSource:
    AccelerationControl = 0x88
    Direction = 0x68
    Distance = 0x74
    EmissionRate = 0x50
    EndRadius = 0x7C
    Loop = 0x58
    Period = 0x54
    StartOrbitPhase = 0x80
    StartPoint = 0x5C
    StartRadius = 0x78
    Turns = 0x84

class ParticleElementParameterByLife:
    CurveGuid = 0x98
    ParameterCurveX = 0x50
    ParameterCurveY = 0x68
    ParameterCurveZ = 0x80

class ParticleElementProperty:
    DelayTime = 0x130
    DelayTimeRandom = 0x134
    DepthOffset = 0x12C
    DepthTest = 0x102
    DepthWrite = 0x103
    Distortion = 0x100
    DynamicBatch = 0x144
    FadeDistance = 0x128
    LODGroup = 0x52
    Local = 0x101
    MaterialInfo = 0x58
    MaxParticles = 0x50
    OffsetScreen = 0x140
    RandomSeed = 0x13C
    RenderSortMode = 0x142
    Rotation = 0x110
    Scale = 0x104
    SortMode = 0x53
    Translation = 0x11C
    WarmUpTime = 0x138

class ParticleElementRandomizer:
    RandomRotation = 0x58
    RandomScale = 0x60
    RandomTranslation = 0x50

class ParticleElementResistance:
    DelayApply = 0x5C
    ResistanceConstant = 0x50
    ResistanceLinear = 0x54
    ResistanceQuadratic = 0x58

class ParticleElementRevive:
    ReviveLife = 0x50
    TargetLife = 0x54

class ParticleElementRotationByLife:
    CurveGuid = 0x68
    RotationCurve = 0x50

class ParticleElementScatter:
    Normalized = 0x58
    SourceDistribution = 0x50
    VelocityScale = 0x5C

class ParticleElementSetter:
    EventName = 0x50
    SetColor = 0x68
    SetLife = 0x69
    SetPosition = 0x6A
    SetRotation = 0x6C
    SetSize = 0x6B
    SetVelocity = 0x6D

class ParticleElementSize2DByLife:
    CoupledMode = 0x98
    CurveGuid = 0x80
    SizeCurveX = 0x50
    SizeCurveY = 0x68

class ParticleElementSize3DByLife:
    Constraint = 0x50
    CoupledMode = 0xB8
    CurveGuid = 0xA0
    SizeCurveX = 0x58
    SizeCurveY = 0x70
    SizeCurveZ = 0x88

class ParticleElementSize3DByTime:
    CurveGuid = 0xA0
    Loop = 0x54
    OverLife = 0x55
    Peroid = 0x50
    SizeCurveX = 0x58
    SizeCurveY = 0x70
    SizeCurveZ = 0x88

class ParticleElementSizeByLife:
    CoupledMode = 0x80
    CurveGuid = 0x68
    SizeCurve = 0x50

class ParticleElementSizeByTime:
    CurveGuid = 0x70
    Loop = 0x54
    OverLife = 0x55
    Peroid = 0x50
    SizeCurve = 0x58

class ParticleElementSizeByVelocity:
    CoupledMode = 0x94
    CurveGuid = 0x68
    SizeCurve = 0x50
    StretchScale = 0x88
    VelocityMax = 0x84
    VelocityMin = 0x80

class ParticleElementSource:
    EmissionDelay = 0x58
    EmissionDuration = 0x54
    EmissionRate = 0x50

class ParticleElementSpinByLife:
    CoupledMode = 0xB0
    CurveGuid = 0x98
    PitchCurve = 0x50
    RollCurve = 0x80
    YawCurve = 0x68

class ParticleElementSpinByVelocity:
    BionicMotion = 0x52
    InitializeOnly = 0x51
    Orientation = 0x50

class ParticleElementSubUVByLife:
    Loop = 0x53
    TileStartMax = 0x52
    TileStartMin = 0x51
    TilesPerSecond = 0x50

class ParticleElementSubUVCurve:
    CurveGuid = 0x68
    Loop = 0x78
    SubUVCurve = 0x50

class ParticleElementSustain:
    Attack = 0x50
    Release = 0x54

class ParticleElementTarget:
    AutoKill = 0x7D
    Speed = 0x78
    Target = 0x50
    TargetLocal = 0x7C
    TargetParameter = 0x60

class ParticleElementTrailSource:
    InheritVelocity = 0x6C
    InheritVelocityScale = 0x70
    SourceName = 0x50
    TrailInterval = 0x68

class ParticleElementTrigger:
    Event = 0x50
    EventName = 0x58
    InheritColor = 0x79
    InheritLife = 0x7A
    InheritRotation = 0x7C
    InheritSize = 0x7B
    InheritVelocity = 0x7D
    InheritVelocityScale = 0x80
    KeepActive = 0x78
    SpawnCount = 0x70

class ParticleElementTurbulence:
    PhaseDistribution = 0x60
    SpaceFreqency = 0x54
    TimeFreqency = 0x58
    Turbulence = 0x50

class ParticleElementTypeDataBeam:
    AttackDistance = 0x9C
    AxialVariation = 0x98
    ColorControl = 0xE0
    EnableControl = 0xC4
    EnableTangents = 0xA4
    FixedSubdivisions = 0x51
    FixedTextureRepeat = 0x50
    NoiseLevel = 0x64
    NoiseScales = 0x68
    NoiseWeights = 0x78
    RadialByVelocity = 0x94
    RadialVariation = 0x90
    ReleaseDistance = 0xA0
    SizeControl = 0xC8
    SourceTangent = 0xA8
    SpaceFrequency = 0x88
    SubdivisionCount = 0x5C
    SubdivisionPeriod = 0x60
    TangentAdaptive = 0xA5
    TargetTangent = 0xB0
    TextureRepeatCount = 0x54
    TextureRepeatPeriod = 0x58
    TimeFrequency = 0x8C
    UVPanningRate = 0xBC
    UVPanningTailsExtra = 0xC0
    UVScollingSpeed = 0xB8

class ParticleElementTypeDataBlade:
    DistanceSampleInterval = 0x50
    FixOrigin = 0x68
    MaxSampleCount = 0x5C
    RepeatU = 0x69
    RepeatV = 0x6A
    RotationSampleInterval = 0x54
    SizeSampleInterval = 0x58
    Tension = 0x60
    Thickness = 0x64

class ParticleElementTypeDataCombo:
    LifeMode = 0x50
    SubEmitters = 0x58

class ParticleElementTypeDataDecal:
    LiftDistance = 0x9C
    ProjectDirection = 0xA4
    ProjectDistance = 0x98
    ProjectOnDynamic = 0xA1
    ProjectOnStatic = 0xA0
    SlopeTestEnable = 0xA2

class ParticleElementTypeDataGPU:
    BoundingBox = 0x60
    Capacity = 0x50
    Facing = 0x54
    Scale = 0x58

class ParticleElementTypeDataLight:
    Attenuation = 0x5C
    ColorScale = 0x50
    DeferredOnly = 0x61
    Important = 0x60
    Quality = 0x62

class ParticleElementTypeDataMesh:
    CenterOffset = 0x68
    DoubleSided = 0x76
    Effect = 0x74
    FaceCamera = 0x78
    FixedRotation = 0x79
    MeshPath = 0x50
    PostBlend = 0x77
    RepeatU = 0x7B
    RepeatV = 0x7C
    Solid = 0x75
    YAxisLocked = 0x7A

class ParticleElementTypeDataModel:
    Animation = 0x80
    DoubleSided = 0x9B
    Effect = 0x98
    FaceCamera = 0x9D
    FixedRotation = 0x9E
    HideTPos = 0xA4
    Loop = 0xA2
    ModelPath = 0x50
    OriginalMaterial = 0x99
    PlaySpeed = 0xA8
    PostBlend = 0x9C
    RemoveMotion = 0xA3
    RepeatU = 0xA0
    RepeatV = 0xA1
    SkeletonPath = 0x68
    Solid = 0x9A
    YAxisLocked = 0x9F

class ParticleElementTypeDataSpline:
    Animation = 0x78
    FixedTextureRepeat = 0xB0
    NodeParameters = 0x98
    Parameterized = 0x90
    SkeletonPath = 0x60
    Subdivisions = 0x50
    TextureRepeatCount = 0xB4
    TextureRepeatPeriod = 0xB8
    TextureScolling = 0xBC
    Thickness = 0x58
    Width = 0x54

class ParticleElementTypeDataSprite:
    Facing = 0x50
    Patch = 0x68
    Pivot = 0x5C
    RepeatU = 0x51
    RepeatV = 0x52
    Scale = 0x54
    ScaleFactorParameter = 0x80

class ParticleElementTypeDataTrail:
    Facing = 0x50
    FixOrigin = 0x51
    RepeatU = 0x58
    RepeatV = 0x59
    SpaceFixedUV = 0x52
    TextureRepeatPeriod = 0x54

class ParticleElementTypeDataVisualGraph:
    OverrideExposedParameters = 0x60
    Resource = 0x50

class ParticleElementVelocityByLife:
    CurveGuid = 0x98
    VelocityCurveX = 0x50
    VelocityCurveY = 0x68
    VelocityCurveZ = 0x80

class ParticleEmitter:
    Elements = 0x50
    Enable = 0x40

class ParticleEventInfo:
    Collision = 0x54
    Color = 0x3C
    Emission = 0x4C
    Index = 0xA
    Life = 0x50
    Position = 0xC
    Rotation = 0x24
    Size = 0x18
    Velocity = 0x30

class ParticleMaterialInfo:
    Flags = 0xA0
    Parameters = 0x78
    Shader = 0x10
    TextureCurves = 0x50
    Textures = 0x28

class ParticleParameterConstantFloat:
    Value = 0x40

class ParticleParameterConstantGuid:
    Value = 0x40

class ParticleParameterConstantMatrix:
    Value = 0x40

class ParticleParameterConstantRotator:
    Value = 0x40

class ParticleParameterConstantVector2:
    Value = 0x40

class ParticleParameterConstantVector3:
    Value = 0x40

class ParticleParameterConstantVector4:
    Value = 0x40

class ParticleParameterNodeTransform:
    Entity = 0x40
    Node = 0x48

class ParticleSystem:
    Emitters = 0x40
    Property = 0x58

class ParticleSystemProperty:
    Bias = 0x30
    DelayTime = 0x6D46046000
    EventParam = 0x6D46046000
    FadeOutTime = 0x6D46046000
    FadeOutType = 0x6D46046000
    FollowEntity = 0x3C
    ForcePrinciple = 0x3E
    InheritVisibility = 0x3F
    StopEnd = 0x3D
    StopImmediately = 0x40
    Switches = 0x18

class Physics:
    AeroplaneFilter.GetDriveRB = 0x6D46072000
    AeroplaneFilter.InputState = 0x6D46072000
    AeroplaneFilter.Reset = 0x6D46072000
    AeroplaneFilter.SetDriveRB = 0x6D46072000
    CharCtrlComponent.CheckOverlap = 0x6D46072000
    CharCtrlComponent.CheckProneChange = 0x6D46072000
    CharCtrlComponent.CheckResizeHeight = 0x6D46072000
    CharCtrlComponent.CreateBoxCharCtrl = 0x6D46072000
    CharCtrlComponent.CreateCapsuleCharCtrl = 0x6D46072000
    CharCtrlComponent.CreateRotatableCharCtrl = 0x6D46072000
    CharCtrlComponent.CreateRotatableCharCtrlBox = 0x6D46072000
    CharCtrlComponent.GetEnableRideVelocity = 0x6D46072000
    CharCtrlComponent.GetEnableSlidingOnDynamic = 0x6D46072000
    CharCtrlComponent.GetIsConstraintClimbing = 0x6D46072000
    CharCtrlComponent.GetIsSlidingOnSlope = 0x6D46072000
    CharCtrlComponent.GetNotifyOverlap = 0x6D46072000
    CharCtrlComponent.GetOverlapRecoverWithChar = 0x6D46072000
    CharCtrlComponent.GetOverlapRecoverWithScene = 0x6D46072000
    CharCtrlComponent.GetRideVelocity = 0x6D46072000
    CharCtrlComponent.GetRideVelocityDamping = 0x6D46072000
    CharCtrlComponent.IsOnNonWalkable = 0x6D46072000
    CharCtrlComponent.IsProne = 0x6D46072000
    CharCtrlComponent.IsSimSimplified = 0x6D46072000
    CharCtrlComponent.ResizeBoxCharCtrl = 0x6D46072000
    CharCtrlComponent.ResizeCapsuleCharCtrl = 0x6D46072000
    CharCtrlComponent.ResizeHeight = 0x6D46072000
    CharCtrlComponent.ResizeRotatableCharCtrl = 0x6D46072000
    CharCtrlComponent.SetAsyncUserDataFloat = 0x6D46072000
    CharCtrlComponent.SetAsyncUserDataInt = 0x6D46072000
    CharCtrlComponent.SetAsyncUserDataString = 0x6D46072000
    CharCtrlComponent.SetControllerShapeUp = 0x6D46072000
    CharCtrlComponent.SetEnableRideVelocity = 0x6D46072000
    CharCtrlComponent.SetEnableSlidingOnDynamic = 0x6D46072000
    CharCtrlComponent.SetIsConstraintClimbing = 0x6D46072000
    CharCtrlComponent.SetIsSlidingOnSlope = 0x6D46072000
    CharCtrlComponent.SetNotifyOverlap = 0x6D46072000
    CharCtrlComponent.SetOverlapRecoverWithChar = 0x6D46072000
    CharCtrlComponent.SetOverlapRecoverWithScene = 0x6D46072000
    CharCtrlComponent.SetProne = 0x6D46072000
    CharCtrlComponent.SetRideVelocity = 0x6D46072000
    CharCtrlComponent.SetRideVelocityDamping = 0x6D46072000
    CharCtrlComponent.TeleportTo = 0x6D46072000
    CharCtrlComponentNew.GetKinematicDetect = 0x6D46072000
    CharCtrlComponentNew.GetMTDPriority = 0x6D46072000
    CharCtrlComponentNew.GetShapeCenterPos = 0x6D46072000
    CharCtrlComponentNew.GetStandOnMoving = 0x6D46072000
    CharCtrlComponentNew.GetStaticMTDPriority = 0x6D46072000
    CharCtrlComponentNew.GetTargetTransform = 0x6D46072000
    CharCtrlComponentNew.GetUseNewMTD = 0x6D46072000
    CharCtrlComponentNew.Rescale = 0x6D46072000
    CharCtrlComponentNew.SetIsUpdatedThisFrame = 0x6D46072000
    CharCtrlComponentNew.SetKinematicDetect = 0x6D46072000
    CharCtrlComponentNew.SetMTDPriority = 0x6D46072000
    CharCtrlComponentNew.SetNeedUpdateEntity = 0x6D46072000
    CharCtrlComponentNew.SetPosWithoutResetDroppingVel = 0x6D46072000
    CharCtrlComponentNew.SetStandOnMoving = 0x6D46072000
    CharCtrlComponentNew.SetStaticMTDPriority = 0x6D46072000
    CharCtrlComponentNew.SetUseNewMTD = 0x6D46072000
    CharCtrlComponentNew.UpdateCharCtrl = 0x6D46072000
    CommonVehicleController.ForceGearTo = 0x6D46072000
    ConstraintComponent.Flush = 0x6D46072000
    ConstraintComponent.GetBody = 0x6D46072000
    ConstraintComponent.SetBody = 0x6D46072000
    DestructFamilyFilter.Add = 0x6D46072000
    DestructFamilyFilter.IsStart = 0x6D46072000
    DestructFamilyFilter.Remove = 0x6D46072000
    DestructFamilyFilter.Tick = 0x6D46072000
    DestructFilter.Add = 0x6D46072000
    DestructFilter.Remove = 0x6D46072000
    DestructFilter.Tick = 0x6D46072000
    PhysicsSpaceComponent.ActivateRegion = 0x6D46072000
    PhysicsSpaceComponent.AddAuxFocusArea = 0x6D46072000
    PhysicsSpaceComponent.AddPosReadyCallback = 0x6D46072000
    PhysicsSpaceComponent.AllOverlap = 0x6D46072000
    PhysicsSpaceComponent.AllOverlapFD = 0x6D46072000
    PhysicsSpaceComponent.AllRaycast = 0x6D46072000
    PhysicsSpaceComponent.AllRaycastFD = 0x6D46072000
    PhysicsSpaceComponent.AllSweep = 0x6D46072000
    PhysicsSpaceComponent.AllSweepFD = 0x6D46072000
    PhysicsSpaceComponent.AllSweepWithoutTrigger = 0x6D46072000
    PhysicsSpaceComponent.AllSweepWithoutTriggerFD = 0x6D46072000
    PhysicsSpaceComponent.ClearAuxFocusAreas = 0x6D46072000
    PhysicsSpaceComponent.ClosestRaycast = 0x6D46072000
    PhysicsSpaceComponent.ClosestRaycastFD = 0x6D46072000
    PhysicsSpaceComponent.ClosestRaycastWithoutTrigger = 0x6D46072000
    PhysicsSpaceComponent.ClosestRaycastWithoutTriggerFD = 0x6D46072000
    PhysicsSpaceComponent.ClosestSweep = 0x6D46072000
    PhysicsSpaceComponent.ClosestSweepFD = 0x6D46072000
    PhysicsSpaceComponent.ClosestSweepWithoutTrigger = 0x6D46072000
    PhysicsSpaceComponent.ClosestSweepWithoutTriggerFD = 0x6D46072000
    PhysicsSpaceComponent.ExportSceneToFile = 0x6D46072000
    PhysicsSpaceComponent.GetAuxFocusAreaCenters = 0x6D46072000
    PhysicsSpaceComponent.GetCharctrlInRange = 0x6D46072000
    PhysicsSpaceComponent.GetCharctrlInRangeWithFilterInfo = 0x6D46072000
    PhysicsSpaceComponent.IsInTick = 0x6D46072000
    PhysicsSpaceComponent.IsPosReady = 0x6D46072000
    PhysicsSpaceComponent.IsUseVehicle33 = 0x6D46072000
    PhysicsSpaceComponent.QueryGroundPos = 0x6D46072000
    PhysicsSpaceComponent.QueryGroundPosFD = 0x6D46072000
    PhysicsSpaceComponent.RaycastClosest = 0x6D46072000
    PhysicsSpaceComponent.SetUseVehicle33 = 0x6D46072000
    PhysicsSpaceComponent.UpdateFocus = 0x6D46072000
    PhysicsSpaceDisplayerComponent.Refresh = 0x6D46072000
    QuatFilter.Input = 0x6D46072000
    QuatFilter.IsStart = 0x6D46072000
    QuatFilter.Reset = 0x6D46072000
    QuatFilter.Start = 0x6D46072000
    QuatFilter.Tick = 0x6D46072000
    RagdollComponent.GetAITrainingStateData = 0x6D46072000
    RagdollComponent.GetBonesInHighResSkeleton = 0x6D46072000
    RagdollComponent.SetAITrainingActionData = 0x6D46072000
    RagdollComponent.SetAITrainingConfig = 0x6D46072000
    RigidBodyComponent.ApplyImpulse = 0x6D46072000
    RigidBodyComponent.ApplyImpulseAtPos = 0x6D46072000
    RigidBodyComponent.ApplyTorque = 0x6D46072000
    RigidBodyComponent.CreateWithShape = 0x6D46072000
    RigidBodyComponent.CreateWithShapeAndMaterial = 0x6D46072000
    RigidBodyComponent.CreateWithShapes = 0x6D46072000
    RigidBodyComponent.GetEnableInterpolatePose = 0x6D46072000
    RigidBodyComponent.GetShapes = 0x6D46072000
    RigidBodyComponent.SetEnableInterpolatePose = 0x6D46072000
    RigidBodyComponent.SetFollowTarget = 0x6D46072000
    RigidBodyComponent.SetKinematicTransform = 0x6D46072000
    RigidBodyComponent.SetKinematicTransformWithOffsetConvert = 0x6D46072000
    RigidBodyComponent.SetMaterial = 0x6D46072000
    VehicleStateFilter.GetDriveRB = 0x6D46072000
    VehicleStateFilter.InputState = 0x6D46072000
    VehicleStateFilter.Reset = 0x6D46072000
    VehicleStateFilter.SetDriveRB = 0x6D46072000
    VehicleStateFilterEx.InputState = 0x6D46072000
    VehicleStateFilterEx.Log = 0x6D46072000
    VehicleStateFilterEx.ResetPosition = 0x6D46072000
    VehicleStateFilterEx.SetStateReceiver = 0x6D46072000
    VehicleStateFilterUs.CalculateWheelRotation = 0x6D46072000
    VehicleStateFilterUs.CalculateWheelSteer = 0x6D46072000
    VehicleStateFilterUs.InputState = 0x6D46072000
    VehicleStateFilterUs.Reset = 0x6D46072000
    VehicleStateSender.SetStateReceiver = 0x6D46072000

class PhysicsCDBodyDebugDrawer:
    EnableCollisionGroupColor = 0x62
    Show = 0x60
    ShowCMLocalFrame = 0x61

class PhysicsCollidableComponent:
    CollisionFilterInfo = 0x174
    IsResourceReady = 0x1D9
    IsSceneComponent = 0x1D8
    OffsetTransform = 0x178

class PhysicsCollisionAwareSceneBody:
    LocalBound = 0x138
    TemplateRes = 0x118

class PhysicsDynamicSceneBody:
    LocalBound = 0x22C
    ParentEntLevel = 0x1F0
    TemplateRes = 0x1E0

class PhysicsEngineSettings:
    DMaxDamageThreshold = 0x10
    DMinDamageThreshold = 0xC
    DynamicFriction = 0x18
    Restitution = 0x1C
    StaticFriction = 0x14

class PhysicsMaterialConfig:
    DynamicFraction = 0x44
    Restitution = 0x48
    StaticFraction = 0x40

class PhysicsMaterialData:
    FrictionCombineType = 0xC
    RestitutionCombineType = 0xD
    TypeID = 0x10

class PhysicsRigidBodyTemplateData:
    CollisionProperties = 0x2C
    DynamicProperties = 0x50
    MaterialGuids = 0x70
    MotionType = 0x68
    Shapes = 0x88

class PhysicsShapeConfig:
    ShapeType = 0x58
    UseShapeCollProps = 0x80
    Vertices = 0x40

class PhysicsSpaceComponent:
    ChunkEdgeLength = 0x20C
    ChunkHalfHeight = 0x210
    CurrentFocusCenter = 0x1F8
    DefaultFocusCenter = 0x1EC
    Displayer = 0x220
    EnablePhysicsLoading = 0x6D46046000
    EnableUpdateFocusCenter = 0x208
    FocusHalfChunkSize = 0x214
    FocusSource = 0x204
    ForceLoadAllRigidBodies = 0x228
    MaxTickPerFrame = 0x1E8
    PvdIpToConnect = 0x230
    ReconnectToPvd = 0x248
    WorldHalfSize = 0x218

class PhysicsSpaceDisplayerComponent:
    AutoUpdate = 0xE4
    DebugVisualHalfExtent = 0xD8
    Display = 0xD4
    HeightOffset = 0xC8
    Space = 0x120
    UpdateVisual = 0xE5
    XOffset = 0xCC
    ZOffset = 0xD0

class PhysicsStaticSceneBody:
    LocalBound = 0x1D8
    OffsetTransform = 0x1A8
    ParentEntLevel = 0x200
    TemplateRes = 0x198

class PhysicsTerrainComponent:
    StaticSceneBodies = 0x70
    TemplateRes = 0x60

class PhysicsTerrainTemplateData:
    ChunkRBLocalBounds = 0x18

class PipeBlender:
    ApplyMotorResult = 0xE8
    EscapedTime = 0xEC
    Finished = 0xE9
    Invalid = 0x9C
    Parallel = 0xEA
    PlacerGroup = 0xF8
    RuntimeInfo = 0xB8
    TotalTime = 0xF0

class PivotPlacer:
    AngularRatio = 0xE4
    AngularTolerance = 0xF4
    Aspect = 0x11C
    Direction = 0x138
    EscapedTime = 0xD8
    Finished = 0xC0
    FocusPosOffset = 0x12C
    FocusRatio = 0xE8
    FocusTolerance = 0xF8
    Fov = 0x118
    HalfFovH = 0x120
    HalfFovV = 0x124
    InsideScreen = 0xE2
    LinkAngle = 0x10C
    LinkAngleEpsilon = 0x104
    LinkAngleThreshold = 0x108
    MajorTarget = 0x178
    MajorTargetPos = 0x144
    MajorTargetPosOffset = 0x15C
    MajorTargetTransform = 0x188
    ManualRotate = 0xC1
    MaxDistance = 0xB8
    MinDistXz = 0x110
    MinLinkDist = 0xBC
    MinorTarget = 0x180
    MinorTargetPos = 0x150
    MinorTargetPosOffset = 0x168
    MinorTargetTransform = 0x1B8
    PitchAngle = 0x114
    PitchMax = 0xC8
    PitchMin = 0xC4
    PivotRatio = 0x128
    RadialRatio = 0xEC
    RadialTolerance = 0xFC
    ResetPhase = 0xE0
    TotalTime = 0xDC
    UseScreenAngle = 0xE1
    VerticalRatio = 0xF0
    VerticalTolerance = 0x100
    ZoomHalflife = 0xD4
    ZoomRatio = 0xD0

class PlatformCondition:
    Predicate = 0x18
    Value = 0x30

class PlatformConditionGroup:
    Conditions = 0x18

class PlatformSection:
    ConditionGroup = 0x48
    Default = 0x30
    Value = 0x18

class PluginModule:
    DefaultLoad = 0x30
    Image = 0x18

class PointCloudComponent:
    Enable = 0x152
    Resource = 0x158

class PointLightComponent:
    Color = 0x13C
    FilterRange = 0x160
    IESTexture = 0x168
    Intensity = 0x14C
    InverseSquaredFalloff = 0x154
    IsSphericalLight = 0x155
    Lumens = 0x150
    Radius = 0x15C
    Range = 0x158
    Temperature = 0x148

class PolygonLocalEnvVolumeComponent:
    ControlPoints = 0xA08
    Enable = 0xA01
    FadeDistance = 0xA04

class PoseSenderComponent:
    AccTime = 0xE0
    DebugOutput = 0xA0
    DebugOutput2 = 0xA1
    EnableSend = 0x91
    EnableSendGraphState = 0x90
    GraphSendDelta = 0x84
    PlatformEntity = 0xA8
    PredictFadeIn = 0x98
    PredictFadeOut = 0x9C
    PredictMoveDeltaTime = 0x94
    SendInterval = 0x80
    ThresholdSpeed = 0x88
    ThresholdYawSpeed = 0x8C

class ProductInfo:
    Bid = 0x50
    Name = 0x18
    Price = 0x30
    Ratio = 0x34
    SdkPids = 0x38

class Profiler(timer=None, timeunit=None, subcalls=True, builtins=True)

    Builds a profiler object using the specified timer function:
    The default timer is a fast built-in one based on real time.
    For custom timer functions returning integers, timeunit can
    be a float specifying a scale (i.e. how long each integer unit
    is, in seconds). = 0x10

class Puppet:
    SpineComponent.AddAnimation = 0x6D46072000
    SpineComponent.AttachParticle = 0x6D46072000
    SpineComponent.DetachParticle = 0x6D46072000
    SpineComponent.GoToNextTime = 0x6D46072000
    SpineComponent.LoadResource = 0x6D46072000
    SpineComponent.OverrideShader = 0x6D46072000
    SpineComponent.SetAlpha = 0x6D46072000
    SpineComponent.SetAnimation = 0x6D46072000
    SpineComponent.SetAnimationAtLastFrame = 0x6D46072000
    SpineComponent.SetEmptyAnimation = 0x6D46072000
    SpineComponent.SetEmptyAnimations = 0x6D46072000
    SpineComponent.SetMix = 0x6D46072000
    SpineComponent.SetMixBlend = 0x6D46072000
    SpineComponent.SetSkin = 0x6D46072000
    SpineComponent.SetToSetupPose = 0x6D46072000
    SpineComponent.UpdateBoolValue = 0x6D46072000
    SpineComponent.UpdateFloatValue = 0x6D46072000
    SpineComponent.UpdateLocalTransform = 0x6D46072000
    SpineComponent.UpdateNormalMaps = 0x6D46072000
    SpineComponent.UpdateTintColor = 0x6D46072000
    SpineComponent.UpdateVector4Value = 0x6D46072000
    SpineComponent.UseCustomStencil = 0x6D46072000

class Python:
    SceneNodeComponent.CreateConnectedNode = 0x6D46072000
    SceneNodeComponent.CreateSceneNode = 0x6D46072000
    SceneNodeComponent.RemoveConnectedNode = 0x6D46072000
    SceneNodeComponent.RemoveSceneNode = 0x6D46072000
    SceneNodeComponent.SeekSceneNode = 0x6D46072000
    SceneNodeComponent.SetConnectedNodeScale9Enabled = 0x6D46072000
    SceneNodeComponent.SetEndpoint = 0x6D46072000
    SceneNodeComponent.SetSceneNodeInfo = 0x6D46072000
    SceneNodeComponent.SetVisible = 0x6D46072000

class QueryPosRes:
    IsValid = 0x30
    ResPos = 0x18

class RBShapeData:
    EnableShapeCollProps = 0x64
    LocalTrans = 0x10
    ShapeCollProps = 0x40

class RBStateSenderDebugComponent:
    HeightOffset = 0xD8
    PointBufferSize = 0xDC
    PointRadius = 0xD4
    SyncFrameColor = 0xC8

class RBStateSyncerDebugComponent:
    BlendInColor = 0x208
    BlendOutColor = 0x214
    BlendPointBufferSize = 0x261
    EstColor = 0x1FC
    EstPointBufferSize = 0x260
    TrajColor = 0x1F0
    TrajPointBufferSize = 0x262

class RagdollBone:
    LockTranslation = 0x18

class RagdollComponent:
    RagdollResPath = 0x258

class RagdollConstraintData:
    LinearLimitDamping = 0xC4
    LinearLimitExtend = 0xBC
    LinearLimitStiffness = 0xC0
    SwingLimitDamping = 0xD4
    SwingLimitStiffness = 0xD0
    SwingLimitY = 0xC8
    SwingLimitZ = 0xCC
    SwingYMotion = 0xB9
    SwingZMotion = 0xBA
    TwistLimitDamping = 0xE4
    TwistLimitHigh = 0xDC
    TwistLimitLow = 0xD8
    TwistLimitStiffness = 0xE0
    TwistMotion = 0xB8
    XAxisMotion = 0xB5
    YAxisMotion = 0xB6
    ZAxisMotion = 0xB7

class RagdollData:
    Constraints = 0x38
    DisableCollisionPairs = 0x80
    HighToLowMapper = 0x108
    LowToHighMapper = 0x98
    Parents = 0x20
    RBToBone = 0x50
    RBToBoneTrans = 0x68

class RagdollRBData:
    ColProp = 0x40
    DynProp = 0x64
    InitPose = 0x7C
    MassProp = 0x14
    MotionType = 0xAC
    Shapes = 0xB0

class RagdollRBShapeData:
    Pos = 0x20
    Rot = 0x10

class RagdollSkeleton:
    Bones = 0x18
    ReferencePose = 0x30

class RecastMapAutoLoader:
    FocusEntity = 0xE0

class RecastMapDisplayerComponent:
    Display = 0xF4
    HeightOffset = 0xF0
    LayerId = 0xFC
    QueryFlag = 0xF8
    ShowModeFlags = 0x100

class RecastNavigatorDebugDrawer:
    Display = 0xD0
    Space = 0xC8

class RectLightComponent:
    Color = 0x13C
    Height = 0x158
    IESTexture = 0x160
    IsDisk = 0x15D
    Lumens = 0x14C
    Range = 0x150
    Temperature = 0x148
    TwoSided = 0x15C
    Width = 0x154

class ReflectionProbeComponent:
    AsSky = 0x16A
    AttenDistance = 0x180
    Enable = 0x168
    EnableTimeOfDay = 0x16B
    Guid = 0x148
    GuidArray = 0x158
    IBL_Diffuse = 0x188
    IBL_SH = 0x1D0
    InShadow = 0x16C
    IsDefault = 0x169
    MipLayers = 0x184
    OffsetPos = 0x170
    ReflectionDistance = 0x17C
    TimeOfDays = 0x240

class RenderOptions:
    AOBase = 0x148
    AOBiasDynamic = 0x144
    AOBiasStatic = 0x130
    AOFarDynamic = 0x13C
    AOFarStatic = 0x128
    AOLastFrameRateDynamic = 0x11C
    AOLastFrameRateStatic = 0x118
    AONearDynamic = 0x138
    AONearStatic = 0x124
    AORandomMap = 0x14C
    AORangeDynamic = 0x140
    AORangeStatic = 0x12C
    AOScaleDynamic = 0x134
    AOScaleStatic = 0x120
    AOScreenScale = 0x114
    AerialPerspectiveLUTDepthKm = 0xDC
    AerialPerspectiveLUTDepthResolution = 0xD8
    AerialPerspectiveLUTSampleCountMaxPerSlice = 0xE0
    AnisotropicQuality = 0x1C
    AtmosphereForceRayMarching = 0xCF
    AtmosphereLUTDownsampleSize = 0xD4
    AutoExposureMethod = 0x28
    BillboardDistance = 0x274
    BillboardShadowSimulation = 0x31F
    BlendInOutTime = 0x264
    BloomDownsampleSize = 0xB7
    BloomLayers = 0xB6
    BlurThreshold = 0xFC
    CSM_Bias0 = 0x210
    CSM_Bias1 = 0x214
    CSM_Bias2 = 0x218
    CSM_Bias3 = 0x21C
    CSM_Distance0 = 0x1F0
    CSM_Distance1 = 0x1F4
    CSM_Distance2 = 0x1F8
    CSM_Distance3 = 0x1FC
    CSM_Dynamic = 0x1ED
    CSM_FilterRange0 = 0x200
    CSM_FilterRange1 = 0x204
    CSM_FilterRange2 = 0x208
    CSM_FilterRange3 = 0x20C
    CSM_LayerTransitionWidth = 0x230
    CSM_Layers = 0x1EE
    CSM_SlopBias0 = 0x220
    CSM_SlopBias1 = 0x224
    CSM_SlopBias2 = 0x228
    CSM_SlopBias3 = 0x22C
    CandelaBackFaceDistance = 0xA8
    CandelaFilterTimes = 0x45
    CandelaGIType = 0x2E
    CandelaLastFrameRate = 0xAC
    CandelaLimitDiffusionTimes = 0x46
    CandelaMarchingSteps = 0x43
    CandelaMaxSamples = 0x3C
    CandelaProbeCellSize0 = 0x60
    CandelaProbeCellSize1 = 0x68
    CandelaProbeCellSize2 = 0x70
    CandelaProbeCellSize3 = 0x78
    CandelaProbeLayers = 0x5C
    CandelaProbeMarchingSteps = 0x85
    CandelaProbeMinSDF = 0x88
    CandelaProbeXZSize = 0x5A
    CandelaProbeYSize = 0x5B
    CandelaReflection = 0x91
    CandelaReflectionDebug = 0xA4
    CandelaSDFCellSize0 = 0x48
    CandelaSDFCellSize1 = 0x4C
    CandelaSDFCellSize2 = 0x50
    CandelaSDFCellSize3 = 0x54
    CandelaSDFLayers = 0x44
    CandelaSDFTextureSize = 0x38
    CandelaScreenScale = 0x34
    CandelaSurfelBufferMemory = 0x47
    CandelaTraceMethod = 0x31
    CastShadowPointLights = 0x1C3
    CastShadowSpotLights = 0x1C4
    ContactShadowDistance = 0x24C
    ContactShadowFadeDistance = 0x250
    ContactShadowLength = 0x244
    ContactShadowSteps = 0x248
    ContactShadowStrength = 0x258
    ContactShadowWorldSpaceUnits = 0x254
    Debug = 0x1F
    DisableFoliageBake = 0x2FC
    DisableFoliageInstancing = 0x2F3
    DisableLightmapLoad = 0x31E
    DrawFlag = 0x318
    DrawIcon = 0x2ED
    DynamicBlend = 0x190
    EnableAMDFSR = 0xC4
    EnableAMDFSR2 = 0xC5
    EnableAMDFSRSharpen = 0xC6
    EnableAdaption = 0xB3
    EnableAmbientOcclusion = 0x110
    EnableBloom = 0xB5
    EnableBokehDoF = 0xBD
    EnableCandelaGI2 = 0xA5
    EnableCheckerboard = 0x1E
    EnableChromaticAberration = 0xC1
    EnableClipPrePass = 0x2F0
    EnableCloudGodRay = 0xCC
    EnableColorGrading = 0xBA
    EnableCompute = 0xB9
    EnableContactShadow = 0x240
    EnableDLSS = 0xC7
    EnableDetailEnhancement = 0xB8
    EnableDissolve = 0x314
    EnableDoF = 0xBC
    EnableDynamicBatchParticle = 0x21
    EnableDynamicInstancing = 0x20
    EnableEdgeLineNormalGen = 0x2EF
    EnableEnvRendering = 0x1DD
    EnableFixCastRange = 0x19E
    EnableFlare = 0x320
    EnableFluidSim = 0x6D46046000
    EnableFog = 0x1C8
    EnableFoliageIndirect = 0x2F4
    EnableForwardCSM = 0x19D
    EnableFullScreenAR = 0x2FE
    EnableGPUOcclusionCulling = 0x2F5
    EnableGodRay = 0xBE
    EnableGroundTruthBasedAO = 0x14D
    EnableHMotionBlur = 0xF5
    EnableHairSystem = 0x2EC
    EnableHalfHDR = 0xB4
    EnableHeightmapFrustrumCulling = 0x2C1
    EnableHeightmapRendering = 0x2C0
    EnableHelper = 0x6D46046000
    EnableHighQualityMultiScattering = 0xD0
    EnableIAppearanceLog = 0x6D46046000
    EnableIBL = 0x1DE
    EnableLocalExposure = 0x24
    EnableMSAA = 0x17F
    EnableMotionBlur = 0xBF
    EnableMultiBounce = 0x2F
    EnableOcclus = 0x25E
    EnablePLUBloomUpsampling = 0x2D
    EnablePVS = 0x25C
    EnablePathTracer = 0x1BC
    EnablePathTracerVisibility = 0x1BD
    EnablePhysicalLightUnit = 0x22
    EnablePointLightShadow = 0x1BF
    EnableRayCache = 0x30
    EnableRenderToMip = 0x2F1
    EnableSGSR = 0xC8
    EnableSSSRendering = 0xC2
    EnableSceneBlur = 0x104
    EnableScreenDepthMask = 0x2EB
    EnableScreenSpaceDistortion = 0xC0
    EnableScreenSpaceShadow = 0x234
    EnableShadowFrustrumCulling = 0x1A8
    EnableShelterMapFrustrumCulling = 0x1EC
    EnableSheltermap = 0x1DF
    EnableSkinCache = 0x2F6
    EnableSkyAtmosphere = 0xCE
    EnableSkyLightingGI = 0x40
    EnableSkyRender = 0xC9
    EnableSortOpaque = 0x1D
    EnableSpecularGI = 0x42
    EnableStarRender = 0xCA
    EnableSunLightShadow = 0x1D0
    EnableSunSSSTranslucency = 0xC3
    EnableTSAA = 0x182
    EnableUnderWaterWave = 0x2EA
    EnableVBuffer = 0x194
    EnableVignetting = 0xBB
    EnableVirtualTexture = 0x2DC
    EnableVirtualTextureCompression = 0x2DD
    EnableVisualizeHistogram = 0x2C
    EnableVolumetricCloud = 0xCB
    EnableVolumetricFog = 0x1C9
    EnableXRay = 0x1DC
    EnabledHdrDisplay = 0xB1
    EnabledHdrLighting = 0xB0
    ExposeTime = 0xF8
    FixSunShadowPosBias = 0x1AC
    FixSunShadowRange = 0x1A0
    FixSunShadowRange2 = 0x1A4
    FocusDistance = 0x260
    FoliageLodDistanceBias = 0x29C
    FoliageLodDistanceScale = 0x2A4
    FoliageLodMeshDistanceBias = 0x2B8
    ForceNoBakeLight = 0x2FB
    ForceNoneCompressionLightmap = 0x2EE
    FrameExtrapolationMethodID = 0x30C
    FrameGraphBuildMode = 0x324
    FrameGraphSetupMode = 0x322
    GTAODepthRadius = 0x15C
    GTAODistance = 0x168
    GTAOEnableSpatialFilter = 0x17C
    GTAOEnableTemporalFilter = 0x17D
    GTAOEnableVisualize = 0x17E
    GTAOFadeDistance = 0x16C
    GTAOLastFrameRateDynamic = 0x178
    GTAOLastFrameRateStatic = 0x174
    GTAOMaxPixelRadius = 0x160
    GTAOScale = 0x170
    GTAOScreenScale = 0x150
    GTAOSliceNum = 0x154
    GTAOStepNum = 0x158
    GTAOThickness = 0x164
    HMotionBlurLevel = 0xF6
    HarmTextBeforeUI = 0x2E8
    HeightmapAccumulationPerSec = 0x2D4
    HeightmapGenDistance = 0x2AC
    HeightmapHeight = 0x2CC
    HeightmapMaxStep = 0x2D8
    HeightmapRange = 0x2C4
    HeightmapTexSize = 0x2D0
    HeightmapYRange = 0x2C8
    HideDockingMark = 0x2FD
    HideFoliage = 0x6D46046000
    HideMisc = 0x31D
    HideModel = 0x6D46046000
    HideParticle = 0x6D46046000
    HideSelection = 0x6D46046000
    HideTerrain = 0x6D46046000
    HideUI = 0x6D46046000
    IconZOffset = 0x308
    ImpostorDistanceOffset = 0x300
    ImpostorResistDistance = 0x304
    InstancedMeshParticleCapacity = 0x2F8
    IsDynamicRecShadow = 0x315
    JitterOffset = 0x184
    LensFlareBaseResolution = 0x310
    LodDistanceBias = 0x298
    LodDistanceScale = 0x2A0
    LodMaxDisplayLevel = 0x284
    LodMeshFadeTime = 0x278
    LodMeshLoadMaxLevel = 0x28C
    LodMeshLoadMinLevel = 0x288
    LodMinDisplayLevel = 0x280
    LodModelLoadMaxLevel = 0x294
    LodModelLoadMinLevel = 0x290
    MSAASampleCount = 0x180
    MSAASampleQuality = 0x181
    MaxAppearReadyTime = 0x268
    MaxBlurPixels = 0x100
    MaxDecalsPerEmitter = 0x2F9
    MaxDetailLevel = 0x2A8
    MaxFoliageDistance = 0x27C
    MaxFoliageDistanceBias = 0x2BC
    MaxLightsLimit = 0x1C6
    MaxShaderLodLevel = 0xE
    MaxVBufferDrawCalls = 0x19C
    MaxVBufferRange = 0x198
    MinShaderLodLevel = 0xD
    MipFocusDistance = 0x26C
    MipMaxLevel = 0x271
    MipMinLevel = 0x270
    OldLightUnitCompatible = 0x23
    OnlyDrawUI = 0x2E9
    OptimizeProbePos = 0x8F
    PBRLightingQuality = 0x1BE
    PSVLoadRange = 0x2FA
    PVSLoadRange = 0x25D
    PlanarShadow = 0x1A9
    PointLitShadowMapSize = 0x1C0
    PrimitiveAOIRange = 0x2B0
    PrincipleRoleShaderLod = 0xF
    ProbeFramesPerUpdate = 0x84
    ProbeHistoryWeight = 0x80
    ProbeOcclusionTest = 0x8C
    ProbeSHOrder3 = 0x90
    ProxyDistanceBias = 0x2B4
    PvsCulling = 0x6D46046000
    RealtimeReflection = 0x1D4
    RealtimeReflectionSizeScale = 0x1D8
    RealtimeReflectionWithShadow = 0x1D5
    ReflectionBRDFBias = 0x9C
    ReflectionMaxRoughness = 0xA0
    ReflectionProbeMapSize = 0x1D2
    ReflectionScreenScale = 0x94
    ReflectionScreenTrace = 0x98
    RenderIntervalFrames = 0xC
    SSRBRDFBias = 0xE8
    SSRMaxRoughness = 0xEC
    SSRRandomMap = 0xF4
    SSRTraceDistance = 0xF0
    SSRTraceSize = 0xE5
    SSRTraceStep = 0xE6
    SSShadowLength = 0x238
    SSShadowSteps = 0x23C
    SceneBlurBrightness = 0x108
    SceneBlurDownsample = 0x105
    SceneBlurPixels = 0x10C
    ScreenSpaceReflection = 0xE4
    ShaderLodDistance1 = 0x10
    ShaderLodDistance2 = 0x14
    ShaderLodDistance3 = 0x18
    ShadowMapSize = 0x1D1
    ShadowMethod = 0x1C1
    ShadowQuality = 0x1C2
    ShelterMapRange = 0x1E4
    ShelterMapSize = 0x1E0
    ShelterMapYRange = 0x1E8
    ShowInvisiblePrimitive = 0x6D46046000
    ShowStatistics = 0x31C
    SkinCacheDefaultMode = 0x2F7
    SkyLightingSH3 = 0x41
    StaticBlend = 0x18C
    StopProbeUpdate = 0x8E
    StopUpdateSDF = 0x59
    TAAQualityLevel = 0x183
    TAASamples = 0x188
    UseAddInsteadOfMaxForEs2BlendOp = 0x2F2
    UseScreenSizeLod = 0x25F
    VirtualTextureBlendRange = 0x2E0
    VirtualTextureTempRTCount = 0x2E4
    VisualizeOcclusDepth = 0x6D46046000
    VisualizeProbe = 0x8D
    VisualizeSDF = 0x58
    VolumetricCloudSize = 0xCD
    VolumetricFogResolutionScale = 0x1CC

class RigidBodyCollisionProperties:
    QueryFilterData0 = 0x10
    QueryFilterData1 = 0x14
    QueryFilterData2 = 0x18
    QueryFilterData3 = 0x1C
    RBShapeFlags = 0x20
    SimFilterData3 = 0xC

class RigidBodyComponent:
    AngularDamping = 0x2FC
    AngularVel = 0x2EC
    CenterOfMass = 0x304
    ControllEntity = 0x358
    EnableCCD = 0x300
    EnableGravity = 0x31D
    FollowTarget = 0x420
    InertiaTensor = 0x310
    IsTrigger = 0x31C
    LinearDamping = 0x2F8
    LinearVel = 0x2E0
    LocalBound = 0x368
    Mass = 0x354
    MotionType = 0x350
    RbTrans = 0x3F0
    Scale = 0x35C
    ShapePath = 0x3A0
    ShapeRes = 0x3B8
    WorldBound = 0x380

class RigidBodyDynamicProperties:
    DisableGravity = 0x14
    EnableCCD = 0x15
    MaxAngularSpeed = 0xC
    SleepThreshold = 0x10

class RigidBodyMassProperties:
    Mass = 0x28
    MassLocalPoseRot = 0xC
    MassSpaceInertiaTensor = 0x1C

class RoadwayEdgeData:
    A = 0x48
    Controls = 0x78
    D = 0x30
    LGamma = 0x58
    LScale = 0x68
    P = 0x18
    Precision = 0x54
    RGamma = 0x60
    RScale = 0x70
    Radius = 0x50

class RoadwayNodeData:
    LTheta = 0x30
    RTheta = 0x34
    Radius = 0x38

class RotateEasingInfo:
    easingEndPitchYaw = 0x10
    easingFinished = 0x20
    easingFinishedNextFrame = 0x21
    lastEasingPitchYaw = 0x18

class RsTextureSliceInfo:
    PitchInByte = 0xA
    SliceInByte = 0xC

class SDecalData:
    IsProjectDynamic = 0x40
    Mixmap = 0x10
    Normalmap = 0x20
    ShaderGraphDecal = 0x30

class SGPPSerializeableTree:
    fVals = 0x58
    tVals = 0x30

class SHVolumeComponent:
    Enable = 0x172
    Resource = 0x178

class SMaterialData:
    flags = 0x30
    info = 0x18

class SMeshGroup:
    BoundingBox = 0x2C
    BoundingVolume = 0x10
    VerticesCount = 0xC

class SMeshVertexStream:
    StreamSize = 0x10

class SPSVolumeData:
    SizeX = 0x20
    SizeY = 0x24
    SizeZ = 0x28
    TileCounts = 0x2C
    TileHeight = 0x1C
    TileSize = 0x18

class SPVSCubeData:
    BytesPerTile = 0x2C
    IsFourByteId = 0x34
    MaxOccluID = 0x30
    SizeX = 0x1C
    SizeZ = 0x20
    TileCounts = 0x28
    TileHeight = 0x24
    TileSize = 0x18
    Version = 0x38

class SPVSSunShadowData:
    BytesPerTile = 0xC
    IsFourByteId = 0x14
    MaxOccluID = 0x10

class SPointCloudData:
    BGridSizeX = 0x3C
    BGridSizeY = 0x40
    BGridSizeZ = 0x44
    BGridTileSize = 0x38
    CGridSizeX = 0x4C
    CGridSizeY = 0x50
    CGridSizeZ = 0x54
    CGridTileSize = 0x48
    D = 0x34
    L = 0x30
    NPoints = 0x28
    NSamples = 0x2C
    SizeX = 0x1C
    SizeY = 0x20
    SizeZ = 0x24
    TileSize = 0x18

class SRenderMeshData:
    BoundingBox = 0x34
    BoundingVolume = 0x18
    Groups = 0xF0
    IndexCount = 0xC
    Indices = 0xD0
    KeepData = 0x15
    PrimTopology = 0x14
    StreamingMask = 0x10
    Streams = 0x50

class SRenderMeshPyramidNode:
    BoudingBox = 0x2C
    IndicesCount = 0x24
    IndicesCountTotal = 0x28
    StartIndex = 0x20

class SSHVolumeData:
    AxisX = 0xC
    AxisY = 0x18
    AxisZ = 0x24
    BGridSizeX = 0x5C
    BGridSizeY = 0x60
    BGridSizeZ = 0x64
    BGridTileSize = 0x58
    CGridSizeX = 0x6C
    CGridSizeY = 0x70
    CGridSizeZ = 0x74
    CGridTileSize = 0x68
    D = 0x54
    L = 0x50
    LenXYZ = 0x30
    NPoints = 0x4C
    SizeX = 0x40
    SizeY = 0x44
    SizeZ = 0x48
    TileSize = 0x3C

class SSkinBone:
    Name = 0x30

class STerrainChunkData:
    BoundBox = 0x30
    Flags = 0x50
    Position = 0x48

class STerrainData:
    ChunkSampleResolution = 0xE
    ChunkSize = 0x10
    Chunks = 0x18
    Flags = 0x30
    MeshResolution = 0xC

class STerrainVolumeData:
    Flags = 0x1C
    GridResolution = 0xC
    GridSize = 0x18
    Junctions = 0x20

class STerrainVolumeJunctionData:
    Value = 0xC

class SVisibilityCubeData:
    EffectBitCount = 0x2C
    EffectBytesPerTile = 0x30
    MaxOccluId = 0x28
    SampleCount = 0x40
    SizeX = 0x1C
    SizeY = 0x20
    SizeZ = 0x24
    TileSize = 0x18

class SceneGuiderComponent:
    BlendTime = 0xD4
    CachedEntity = 0x120
    CameraPriority = 0x114
    CameraTransform = 0xE4
    Duration = 0xD8
    EpisodeIndex = 0xD0
    FilePath = 0xB8
    FiniRatio = 0xE0
    GuidingType = 0xB0
    InitRatio = 0xDC
    VirtualCamera = 0x118

class SceneNodeComponent:
    AlphaScale = 0xA8
    BorderFlag = 0xB4
    FovDistance = 0xA0
    MaxOpacity = 0xB0
    MaxScale = 0x9C
    MinOpacity = 0xAC
    MinScale = 0x98
    VisibleDistance = 0xA4

class SensorPipe:
    Sensors = 0x88

class SeparateCompCurveObjectVector2:
    Curves = 0x18

class SeparateCompCurveObjectVector3:
    Curves = 0x18

class SeparateCompCurveObjectVector4:
    Curves = 0x18

class Set a callback that will be called when a server name is provided by the SSL/TLS client in the SNI extension:
    If the argument is None then the callback is disabled. The method is called
with the SSLSocket, the server name as a string, and the SSLContext object.
See RFC 6066 for details of the SNI extension. = 0x30

class ShaderGraphDecalMaterialInfo:
    CurveParameters = 0x30
    OutputMask = 0x58
    OverrideParameters = 0x60
    Parameters = 0x18

class ShaderGraphInfo:
    BlendableLocation = 0x74
    BlendablePriority = 0x70
    BranchTreeData = 0x98
    CurveParameters = 0x48
    CustomMask = 0x118
    MaterialProperties = 0xB0
    ParameterCollections = 0xF0
    ParameterEmbeddedName = 0xC8
    PipelineState = 0x75
    VGCustomProperties = 0x148
    VGProperties = 0x120
    VaringMask = 0x90

class ShaderGraphMaterialInfo:
    CurveParameters = 0x48
    DepthOffset = 0xC
    MaterialFlags = 0x10
    ParameterCollections = 0x70
    Parameters = 0x30
    ShaderName = 0x18

class ShaderGraphOverrideMaterialRuntimeInfo:
    MaterialFlagInheritMask = 0x40
    Resource = 0x30

class ShaderGraphPipelineState:
    BlendColorWriteMask = 0x12
    BlendDst = 0x16
    BlendDstAlpha = 0x18
    BlendOp = 0x13
    BlendOpAlpha = 0x14
    BlendSrc = 0x15
    BlendSrcAlpha = 0x17
    StencilDepthFailOp = 0xB
    StencilDepthFailOpBack = 0xF
    StencilFailOp = 0xA
    StencilFailOpBack = 0xE
    StencilFunc = 0xD
    StencilFuncBack = 0x11
    StencilPassOp = 0xC
    StencilPassOpBack = 0x10

class ShaderGraphPostProcessMaterialInfo:
    BlendableLocation = 0x5C
    BlendablePriority = 0x58
    CurveParameters = 0x30
    OverrideLocation = 0x98
    OverrideParameters = 0x80
    OverridePipelineState = 0x99
    Parameters = 0x18
    PipelineState = 0x5D
    VaringMask = 0x78

class ShaderGraphUIMaterialInfo:
    CurveParameters = 0x30
    OverrideParameters = 0x58
    Parameters = 0x18

class ShaderMacro:
    Value = 0x18

class ShaderSettings:
    ExcludeInFinal = 0x18

class ShallowWaterComponent:
    BoundaryDamping = 0x330
    BoundaryHeight = 0x32C
    BoundarySize = 0x328
    EnableLOD = 0x300
    FieldSize = 0x2D0
    FollowTarget = 0x2F8
    GridLength = 0x2D8
    InitState = 0x388
    IsOpenBoundary = 0x324
    LandFieldSize = 0x2E4
    LandGridLength = 0x2EC
    LandPosOffset = 0x2DC
    LodCheckPeriod = 0x320
    LodDistance = 0x308
    LodLevelLocked = 0x378
    TargetLodLevel = 0x37C
    WaterWaveDispScale = 0x360
    WaterWaveGuid = 0x348
    WaterWaveUVScale = 0x358
    WorldPos = 0x2C4

class ShallowWaterData:
    BorderSize = 0x34
    BoundaryDamping = 0x38
    BoundarySurfaceHeight = 0x30
    DeltaTime = 0x2C
    DepthDecayRate = 0x54
    ForceByVelY = 0x40
    GridLength = 0x14
    GridSize = 0xC
    HeightChange = 0x58
    IsOpenSeaBoundary = 0x3C
    LandGridLength = 0x28
    LandGridSize = 0x20
    LandPosOffset = 0x18
    Lift = 0x4C
    LinearDrag = 0x44
    Omega = 0x50
    QuadDrag = 0x48
    VelChange = 0x5C

class ShallowWaterInteractorComponent:
    SubTriangleSizeParam = 0x324
    WaterField = 0x2B0

class ShallowWaterSourceComponent:
    EnableSource = 0x2B8
    WaterField = 0x2B0

class ShallowWaterStateData:
    SurfHeightRange = 0x10
    VelXRange = 0x18
    VelZRange = 0x20

class ShapeCollisionProperties:
    QueryFilterData = 0x10
    ShapeFlags = 0x20

class SimSyncBlendParams:
    B3 = 0xC
    S0 = 0x10
    S1 = 0x14

class SimpleCurveObjectFloat:
    Curve = 0x18

class SimpleCurveObjectVector2:
    Curve = 0x18

class SimpleCurveObjectVector3:
    Curve = 0x18

class SimpleCurveObjectVector4:
    Curve = 0x18

class SingleFocusLocator:
    Direction = 0xA0
    Target = 0xB0
    TargetPos = 0x88
    TargetPosOffset = 0x94
    TargetTransform = 0xB8

class SkeletonComponent:
    CameraName = 0x180
    HideTPose = 0x199
    PlatformEntity = 0x178
    ResourcePath = 0x160
    UseDynamicVisibilityBox = 0x15D

class SkeletonMapperData:
    ChainMappings = 0x38
    KeepUnmappedLocal = 0x68
    MappingType = 0x69
    SimpleMappings = 0x20
    UnmappedBones = 0x50

class SkeletonSystemSetting:
    CharacterCoord = 0x288
    Head = 0xC0
    LCalf = 0x1E0
    LClavicle = 0xD8
    LFinger0 = 0x138
    LFoot = 0x1F8
    LForearm = 0x108
    LHand = 0x120
    LThigh = 0x1C8
    LToe0 = 0x210
    LUpperArm = 0xF0
    Neck = 0xA8
    Pelvis = 0x48
    RCalf = 0x240
    RClavicle = 0x150
    RFinger0 = 0x1B0
    RFoot = 0x258
    RForearm = 0x180
    RHand = 0x198
    RThigh = 0x228
    RToe0 = 0x270
    RUpperArm = 0x168
    Root = 0x18
    RootMode = 0x30
    Spine = 0x60
    Spine1 = 0x78
    Spine2 = 0x90

class SlavedPlacer:
    AngularSpeed = 0xF0
    CameraVelocity = 0xE0
    Direction = 0x11C
    EnableRotate = 0xEC
    EnableUpPos = 0xB8
    EnableYawTrace = 0x13C
    FocusPos = 0xD4
    Fov = 0x10C
    FreeRadius = 0x1A0
    InputLock = 0x154
    MotionDamping = 0x12C
    MotionStiffness = 0x128
    Mover = 0x1A8
    PitchMax = 0xD0
    PitchMin = 0xCC
    PrevTargetPos = 0xF4
    RotationDamping = 0x134
    RotationStiffness = 0x130
    RotationThreshold = 0x138
    Target = 0x1B0
    TargetPos = 0x100
    TargetPosOffset = 0x110
    TargetTransform = 0x1B8
    YawTraceRadius = 0x140
    ZoomHalfLife = 0xC0
    ZoomMax = 0xC8
    ZoomMin = 0xC4

class SnowTrailComponent:
    SnowTrails = 0x88
    SnowTrailsCount = 0x84

class SnowTrailProperty:
    ElevationHeightScale = 0x34
    ElevationRangeScale = 0x30
    Exponent = 0x2C
    InnerRadius = 0x24
    Offset = 0x18
    OuterRadius = 0x28
    Randomness = 0x38

class Sound:
    SoundComponentExt.AddOutputDevice = 0x6D46072000
    SoundComponentExt.DelPortal = 0x6D46072000
    SoundComponentExt.DelRoom = 0x6D46072000
    SoundComponentExt.DelSpatialObstruction = 0x6D46072000
    SoundComponentExt.GetActiveGameObjects = 0x6D46072000
    SoundComponentExt.GetAllGameObjects = 0x6D46072000
    SoundComponentExt.GetEventIDFromPlayingID = 0x6D46072000
    SoundComponentExt.GetFNVHashFromString = 0x6D46072000
    SoundComponentExt.GetGameObjectFromPlayingID = 0x6D46072000
    SoundComponentExt.GetIsGameObjectActive = 0x6D46072000
    SoundComponentExt.GetPlayingIDsFromGameObject = 0x6D46072000
    SoundComponentExt.GetSourcePlayPosition = 0x6D46072000
    SoundComponentExt.Play3DEventWithSwitch = 0x6D46072000
    SoundComponentExt.Play3DEventWithSwitches = 0x6D46072000
    SoundComponentExt.PostTrigger = 0x6D46072000
    SoundComponentExt.RemoveGameObjectParameter = 0x6D46072000
    SoundComponentExt.RemoveOutputDevice = 0x6D46072000
    SoundComponentExt.SetBusConfig = 0x6D46072000
    SoundComponentExt.SetBusEffect = 0x6D46072000
    SoundComponentExt.SetEncryptionKey = 0x6D46072000
    SoundComponentExt.SetGameObjectInRoom = 0x6D46072000
    SoundComponentExt.SetGameObjectObstructionAndOcclusion = 0x6D46072000
    SoundComponentExt.SetGameObjectPosition = 0x6D46072000
    SoundComponentExt.SetGameObjectPositionAndOrientation = 0x6D46072000
    SoundComponentExt.SetGameObjectPositionAndOrientationBias = 0x6D46072000
    SoundComponentExt.SetGameObjectVolume = 0x6D46072000
    SoundComponentExt.SetGameObjectVolumeRamp = 0x6D46072000
    SoundComponentExt.SetGameObjectWhiteList = 0x6D46072000
    SoundComponentExt.SetMediaAsyncCallback = 0x6D46072000
    SoundComponentExt.SetMediaAsyncList = 0x6D46072000
    SoundComponentExt.SetMediaLookUpTable = 0x6D46072000
    SoundComponentExt.SetMuteForCategory = 0x6D46072000
    SoundComponentExt.SetPortal = 0x6D46072000
    SoundComponentExt.SetPortalObstructionAndOcclusion = 0x6D46072000
    SoundComponentExt.SetPriorityForCategory = 0x6D46072000
    SoundComponentExt.SetReflectionsOrder = 0x6D46072000
    SoundComponentExt.SetRoom = 0x6D46072000
    SoundComponentExt.SetScalingFactor = 0x6D46072000
    SoundComponentExt.SetSpatial = 0x6D46072000
    SoundComponentExt.SetSpatialObstructionBox = 0x6D46072000
    SoundComponentExt.SetSpatialObstructionPolygon = 0x6D46072000
    SoundComponentExt.SetState = 0x6D46072000
    SoundComponentExt.SetSwitch = 0x6D46072000
    SoundComponentExt.SetVolumeForCategory = 0x6D46072000
    SoundComponentExt.StartOutputCapture = 0x6D46072000
    SoundComponentExt.StopOutputCapture = 0x6D46072000
    SoundComponentExt.VisualizeSpatialHelpers = 0x6D46072000

class SoundComponent:
    ClampDistanceMap = 0x110
    DeclinePerEvent = 0x100
    DeclinePerMeter = 0x108
    DistanceLimit = 0x104
    EnableClamp = 0xF8
    EventLimitCount = 0xFC
    ListenerPositionBias = 0xF4
    ListenerRole = 0x138
    RolePositionBias = 0xE8
    Rooms = 0x140

class SphereLocalEnvVolumeComponent:
    CenterPos = 0x9AC
    Enable = 0x9A1
    FadeDistance = 0x9A4
    Range = 0x9A8

class SphereShapeData:
    Radius = 0xC

class SpineComponent:
    AmbientLightScale = 0xD8
    LightingEnable = 0xDE
    LockY = 0xDC
    PointLightScale = 0xD0
    SpotLightScale = 0xD4
    SunLightScale = 0xCC
    TimeScale = 0xC8
    WriteDepth = 0xDD

class SpotLightComponent:
    Color = 0x144
    Cookie = 0x178
    CookieRepeats = 0x188
    FilterRange = 0x170
    IESTexture = 0x190
    InnerAngle = 0x168
    Intensity = 0x154
    InverseSquaredFalloff = 0x15C
    Lumens = 0x158
    OutAngle = 0x16C
    Radius = 0x164
    Range = 0x160
    Temperature = 0x150

class StackBlender:
    Finished = 0x98
    SuspendPrevious = 0x99

class SubviewportHelper:
    EnableControl = 0xC2

class Surround 5:
    0 = 0x1
    1 = 0x1

class TachComponent:
    Attachments = 0x68
    Basepoint = 0x98
    CameraAttaching = 0x118
    CameraSpaceTransform = 0x11C
    EnableTachVisible = 0x119
    Hardpoint = 0x80
    Master = 0x60
    Offset = 0xE0
    RotationMode = 0x110
    ScaleMode = 0x114
    Transform = 0xB0

class TagComponent:
    TagString = 0x60

class TerrainChunkComponent:
    Index = 0x70
    OccluId = 0x74
    Offset = 0x68

class TerrainComponent:
    AliasingFactor = 0x484
    AlignToChunk = 0x4BE
    AuxiliaryMap = 0x6D46046000
    AuxiliaryMaps = 0x448
    BlendSharpness = 0x480
    BlockMap = 0x408
    BlockNumber = 0x464
    BlockScale = 0x460
    BlockTags = 0x468
    BloomScale = 0x4C4
    ChunkSize = 0x3A8
    Chunks = 0x3B0
    DisplacementHeight = 0x48C
    EnableOptimization = 0x4BC
    ForbidRotate = 0x4BD
    GeometryFactor = 0x4B0
    IsDisableXRay = 0x4C0
    LightmapScale = 0x3F8
    Lightmaps = 0x3E0
    Materials = 0x3C8
    MaxDetailsDistance = 0x4B4
    MaxLoadingDistance = 0x4B8
    MixtureMap = 0x428
    NormalMap = 0x418
    OverlayMap = 0x438
    Position = 0x398
    Resolution = 0x3A0
    ShaderType = 0x498
    TessFadeBegin = 0x490
    TessFadeEnd = 0x494
    TrianglePerPixel = 0x488
    UseObservationPoints = 0x4BF

class TerrainVolumeComponent:
    Material = 0x310
    MeshLodDistance = 0x324
    MeshUpdateThreshold = 0x320
    UseObservationPoints = 0x328

class TextComponent:
    BoolParameters = 0x378
    CanvasHeight = 0x368
    CanvasWidth = 0x364
    Float2Parameters = 0x3C8
    Float3Parameters = 0x3F0
    Float4Parameters = 0x418
    FloatParameters = 0x3A0
    FontColor = 0x354
    FontName = 0x338
    FontSize = 0x350
    IsBillboard = 0x370
    IsSDFTexture = 0x371
    Margin = 0x36C
    PosBias = 0x2FC
    Shader = 0x320
    Text = 0x308

class Texture2DInfo:
    CompressionNoAlpha = 0x6D46046000
    DefaultColor = 0x10
    Height = 0xE
    IsDynamicRange = 0x6D46046000
    MipGenPreset = 0xA
    NoneCompression = 0x6D46046000
    NoneMipDowngrading = 0x6D46046000
    TextureType = 0xB
    Width = 0xC

class Texture3DEditInfo:
    EnableMipMap = 0x18
    TileSizeX = 0x10
    TileSizeY = 0x14

class Texture3DInfo:
    CompressionNoAlpha = 0x6D46046000
    DefaultColor = 0x14
    Depth = 0x12
    Height = 0x10
    IsDynamicRange = 0x6D46046000
    IsUINT = 0x6D46046000
    LODGroup = 0xA
    MipGenPreset = 0xB
    NoneCompression = 0x6D46046000
    NoneMipDowngrading = 0x6D46046000
    Width = 0xE
    padding = 0x6D46046000

class TextureCompressOption:
    Compression = 0x18
    DownSampleScale = 0xC
    Format = 0x10
    ItemName = 0x50
    KeepRaw = 0x3A
    KeepRawMips = 0x3B
    LinearSpaceResize = 0x3E
    LosslessCompress = 0x3C
    MaxTextureSize = 0x2C
    MipDiyParameter = 0x40
    MipGen = 0x19
    MixRoughenRate = 0x34
    NoneCompression = 0x3D
    NormalSmoothRate = 0x30
    Premultiply = 0x24
    Quality = 0x14
    ResizeFilter = 0x3F
    RoughenMixMap = 0x39
    SRGBIn = 0x1C
    SRGBOut = 0x20
    SeparateAlpha = 0x28
    SmoothNormalMap = 0x38
    SourceFilePath = 0x68

class TextureCurveInfo:
    CurveInfos = 0x18

class The Python-level owner of this object:
    Passed as "self" in servername callback. = 0x10

class The functions in this module allow compression and decompression using the
zlib library, which is based on GNU zip:
    adler32(string[, start]) -- Compute an Adler-32 checksum.
compress(data[, level]) -- Compress data, with compression level 0-9 or -1.
compressobj([level[, ...]]) -- Return a compressor object.
crc32(string[, start]) -- Compute a CRC-32 checksum.
decompress(string,[wbits],[bufsize]) -- Decompresses a compressed string.
decompressobj([wbits[, zdict]]) -- Return a decompressor object.

'wbits' is window buffer size and container format.
Compressor objects support compress() and flush() methods; decompressor
objects support decompress() and flush(). = 0x10

class The io module provides the Python interfaces to stream handling:
    The
builtin open function is defined in this module.

At the top of the I/O hierarchy is the abstract base class IOBase. It
defines the basic interface to a stream. Note, however, that there is no
separation between reading and writing to streams; implementations are
allowed to raise an OSError if they do not support a given operation.

Extending IOBase is RawIOBase which deals simply with the reading and
writing of raw bytes to a stream. FileIO subclasses RawIOBase to provide
an interface to OS files.

BufferedIOBase deals with buffering on a raw byte stream (RawIOBase). Its
subclasses, BufferedWriter, BufferedReader, and BufferedRWPair buffer
streams that are readable, writable, and both respectively.
BufferedRandom provides a buffered interface to random access
streams. BytesIO is a simple stream of in-memory bytes.

Another IOBase subclass, TextIOBase, deals with the encoding and decoding
of streams into text. TextIOWrapper, which extends it, is a buffered text
interface to a buffered raw stream (`BufferedIOBase`). Finally, StringIO
is an in-memory stream for text.

Argument names are not part of the specification, and only the arguments
of open() are intended to be used as keyword arguments.

data:

DEFAULT_BUFFER_SIZE

   An int containing the default buffer size used by the module's buffered
   I/O classes. open() uses the file's blksize (as obtained by os.stat) if
   possible. = 0x10

class The object used to calculate HMAC of a message:
    Methods:

update() -- updates the current digest with an additional string
digest() -- return the current digest value
hexdigest() -- return the current digest as a string of hexadecimal digits
copy() -- return a copy of the current hash object

Attributes:

name -- the name, including the hash algorithm used by this object
digest_size -- number of bytes in digest() output = 0x10

class The time value as returned by gmtime(), localtime(), and strptime(), and
 accepted by asctime(), mktime() and strftime():
    May be considered as a
 sequence of 9 integers.

 Note that several fields' values are not the same as those defined by
 the C language standard for struct tm.  For example, the value of the
 field tm_year is the actual year, not year - 1900.  See individual
 fields' descriptions for details. = 0x9

class This module defines an object type which can efficiently represent
an array of basic values: characters, integers, floating point
numbers:
    Arrays are sequence types and behave very much like lists,
except that the type of objects stored in them is constrained. = 0x10

class This module provides access to operating system functionality that is
standardized by the C Standard and the POSIX standard (a thinly
disguised Unix interface):
    Refer to the library manual and
corresponding Unix manual entries for more information on calls. = 0x10

class This module provides mechanisms to use signal handlers in Python:
    Functions:

alarm() -- cause SIGALRM after a specified time [Unix only]
setitimer() -- cause a signal (described below) after a specified
               float time and the timer may restart then [Unix only]
getitimer() -- get current value of timer [Unix only]
signal() -- set the action for a given signal
getsignal() -- get the signal action for a given signal
pause() -- wait until a signal arrives [Unix only]
default_int_handler() -- default SIGINT handler

signal constants:
SIG_DFL -- used to refer to the system default handler
SIG_IGN -- used to ignore the signal
NSIG -- number of defined signals
SIGINT, SIGTERM, etc. -- signal numbers

itimer constants:
ITIMER_REAL -- decrements in real time, and delivers SIGALRM upon
               expiration
ITIMER_VIRTUAL -- decrements only when the process is executing,
               and delivers SIGVTALRM upon expiration
ITIMER_PROF -- decrements both when the process is executing and
               when the system is executing on behalf of the process.
               Coupled with ITIMER_VIRTUAL, this timer is usually
               used to profile the time spent by the application
               in user and kernel space. SIGPROF is delivered upon
               expiration.


*** IMPORTANT NOTICE ***
A signal handler function is called with two arguments:
the first is the signal number, the second is the interrupted stack frame. = 0x10

class This module provides primitive operations to write multi-threaded programs:
    The 'threading' module provides a more convenient interface. = 0x10

class This module provides various functions to manipulate time values:
    There are two standard representations of time.  One is the number
of seconds since the Epoch, in UTC (a.k.a. GMT).  It may be an integer
or a floating point number (to represent fractions of seconds).
The Epoch is system-defined; on Unix, it is generally January 1st, 1970.
The actual value can be retrieved by calling gmtime(0).

The other representation is a tuple of 9 integers giving local time.
The tuple items are:
  year (including century, e.g. 1998)
  month (1-12)
  day (1-31)
  hours (0-23)
  minutes (0-59)
  seconds (0-59)
  weekday (0-6, Monday is 0)
  Julian day (day in the year, 1-366)
  DST (Daylight Savings Time) flag (-1, 0 or 1)
If the DST flag is 0, the time is given in the regular time zone;
if it is 1, the time is given in the DST time zone;
if it is -1, mktime() should guess based on the date and time. = 0x20

class This module supports asynchronous I/O on multiple file descriptors:
    *** IMPORTANT NOTICE ***
On Windows, only sockets are supported; on Unix, all file descriptors. = 0x10

class TimeOfDayPropKey:
    Data = 0x18
    PropWidth = 0x30

class TimeOfDayPropTrack:
    IsNew = 0x88
    KeyFrameData = 0x30
    KeyFrameStride = 0x8C
    KeyFrameTime = 0x18
    PropWidths = 0x48
    SingleKeyFrames = 0x60

class TpsCollider:
    CollisionFilterInfo = 0x88
    EnableCollider = 0x90
    EnableSmoothIn = 0x92
    EnableSmoothOut = 0x91
    InvalidCollideInfo = 0x9C
    MinDist = 0x8C
    SmoothRatio = 0x98
    SmoothThreshold = 0x94
    SweepShape = 0xE0
    Target = 0x108
    TargetPos = 0xEC
    TargetPosOffset = 0xF8
    TerrainFilterInfo = 0xE8
    UseCameraTargetAsStartPoint = 0x104

class TpsMover:
    EscapedTime = 0x174
    Invalid = 0x178
    MoveTime = 0x170
    PitchMax = 0x16C
    PitchMin = 0x168
    PitchTolerance = 0x164
    TweenMode = 0x179
    YawTolerance = 0x160

class TpsPlacer:
    AdditiveBoneName = 0xD8
    AdditiveFovOffset = 0x1E4
    AdditiveRollOffset = 0x1EC
    AffiliatedFov = 0x1E0
    AlreadyFirstRotate = 0x1F0
    AngularSpeed = 0x1C0
    BoneName = 0xC0
    Direction = 0x200
    Easing = 0x108
    EasingRuntimeInfo = 0x110
    EnableFaceCheck = 0xF8
    EnableInertia = 0x194
    EnableRotate = 0x1B8
    EnableUpPos = 0x148
    FaceThreshold = 0xFC
    FocusDiffTol = 0x19C
    FocusPos = 0x178
    Fov = 0x1DC
    InertiaCoeff = 0x184
    InertiaCoeffAccFrame = 0x1A4
    InertiaCoeffAccReset = 0x1A8
    InertiaCoeffAccStart = 0x1AC
    InertiaCoeffAccType = 0x1B4
    InertiaCoeffCurrent = 0x1B0
    InertiaCoeffDirection = 0x188
    InertiaCoeffHorizon = 0x1A0
    InertiaHalfLife = 0x18C
    InertiaType = 0x198
    Mover = 0x100
    PitchMax = 0x160
    PitchMin = 0x15C
    PrevDirection = 0x20C
    PrevTargetPos = 0x1C4
    Roll = 0x1E8
    Target = 0x218
    TargetDiffTol = 0x1BC
    TargetPos = 0x1D0
    TargetPosOffset = 0x1F4
    TargetTransform = 0x220
    UseBonePosition = 0xB8
    ZoomHalfLife = 0x150
    ZoomMax = 0x158
    ZoomMin = 0x154

class TriangleMeshShapeData:
    Indices = 0x28
    IsDeformable = 0x58
    MaterialIDs = 0x40
    Vertices = 0x10

class TriggerInfo:
    Flag = 0x10

class TumblerConstraintData:
    ImpulseRatio = 0xBC
    MinImpulse = 0xB8

class TwParticleElementForce:
    AlwaysLocal = 0x60
    DelayApply = 0x5C
    Force = 0x50

class TwParticleElementJitter:
    AlwaysLocal = 0x60
    JitterPeriod = 0x64
    PositionJitter = 0x50
    VelocityJitter = 0x58

class TwParticleElementMagnet:
    AlwaysLocal = 0x6C
    DelayApply = 0x64
    Origin = 0x50
    Radius = 0x5C
    Strength = 0x60
    TotalApply = 0x68

class TwParticleElementOrbitor:
    AffectVelocity = 0x6C
    AugularSpeed = 0x68
    Axis = 0x5C
    Origin = 0x50

class TwParticleElementScaler:
    FinalSize = 0x54
    SizeRate = 0x50

class TwParticleElementSource:
    ActiveDuration = 0x64
    EmissionBurst = 0x5C
    EmissionDelay = 0x58
    EmissionDuration = 0x54
    EmissionRate = 0x50
    SleepDuration = 0x68
    TimeTriggered = 0x60

class TwParticleElementStream:
    AlwaysLocal = 0x60
    Direction = 0x54
    HalfLife = 0x50

class TwParticleElementTracker:
    AccelerationControl = 0x94
    AutoClear = 0x5B
    AutoReverse = 0x59
    AutoStop = 0x5A
    Delay = 0x50
    Direction = 0x6C
    Distance = 0x78
    EndRadius = 0x80
    LocalMotion = 0x5C
    Loop = 0x58
    Orientation = 0x5D
    Period = 0x54
    SpinSpeed = 0x90
    StartOrbitPhase = 0x84
    StartPoint = 0x60
    StartRadius = 0x7C
    StartSpinPhase = 0x8C
    Turns = 0x88

class UnifiedPipelineCollisionShapes:
    SkinBone = 0x48
    SphereBoneInd = 0x18
    SphereConnected = 0x30

class UnifiedPipelineSkinBone:
    Name = 0x30

class UnraisableHookArgs

Type used to pass arguments to sys:
    unraisablehook. = 0x5

class UuidInfo:
    EntityName = 0x18
    ID = 0x10

class Vehicle4WheelsTemplate:
    InAirControll = 0x224
    KeySmooth = 0x1A0
    MotorBikeBalance = 0x208
    OAData = 0x240
    PadSmooth = 0x1C8
    RBGuid = 0x10
    SteadyData = 0x274
    SteerRatio = 0x1F0
    VehicleDrive = 0x20
    WheelsSimData = 0x140

class VehicleAckermannGeometry:
    Accuracy = 0x40
    AxleSeparation = 0x4C
    FrontWidth = 0x44
    RearWidth = 0x48

class VehicleAckermannGeometryData:
    AxleSeparation = 0xC

class VehicleAckermannGeometryDesc:
    AxleSeparation = 0xC

class VehicleAntiGravitySuspensionData:
    MaxCompression = 0x30
    MaxDrop = 0x2C
    RestPos = 0x18
    SpringDampingRatio = 0x28
    SpringPeriod = 0x24
    TravelDirection = 0xC

class VehicleAntiRollBar:
    Stiffness = 0x48
    Wheel0 = 0x40
    Wheel1 = 0x44

class VehicleAutoBox:
    DownRatios = 0x60
    Latency = 0x40
    UpRatios = 0x48

class VehicleAutoBoxData:
    DownRatios = 0x3C

class VehicleChassis:
    AngularDamping = 0x60
    CMOffset = 0x50
    LinearDamping = 0x5C
    MOI = 0x40
    Mass = 0x4C
    SleepThreshold = 0x64

class VehicleChassisDesc:
    AngularDamping = 0x20
    CMOffset = 0x10
    LinearDamping = 0x1C
    Mass = 0xC
    SleepThreshold = 0x24

class VehicleClutch:
    Strength = 0x40

class VehicleCollisionModifyData:
    ControllFriction = 0x23
    ControllInvInertiaScale = 0x21
    ControllInvMassScale = 0x22
    ControllMaxImpulse = 0x24
    ControllNormal = 0x25
    ControllRestitution = 0x20
    DynamicFriction = 0x10
    EnableFilter = 0x26
    Enabled = 0x27
    FilterData = 0x1C
    InvMassScale = 0xC
    MaxImpulse = 0x18
    StaticFriction = 0x14

class VehicleCompStateSyncerInterpolation:
    ForceFullSim = 0x55A

class VehicleComponent:
    Accel = 0x510
    BalanceState = 0x238
    Brake = 0x511
    BrakeSound = 0x3A8
    BrakeSoundVolume = 0x3F8
    BrakeThreshold = 0x3DC
    CollisionFilterInfo = 0x6D46046000
    EnableInterpolatePose = 0x51F
    EnableUpdateSkeleton = 0x516
    EnableVehicleEngine = 0x6D46046000
    EngineRunSound = 0x360
    EngineRunSoundVolume = 0x3EC
    EngineSoundRPMRatio = 0x3CC
    EngineSpeedMin = 0x3C8
    EngineSpeedThreshold = 0x3C4
    EngineStartSound = 0x348
    EngineStartSoundVolume = 0x3E8
    ForwardSpeed = 0x3E4
    ForwardSpeedThreshold = 0x3C0
    Handbrake = 0x512
    HoldSoundEvent = 0x51E
    InAirControllParams = 0x248
    InAirControllState = 0x26C
    IsContacted = 0x51B
    OAParams = 0x298
    OAState = 0x2D4
    PadAccel = 0x500
    PadBrake = 0x504
    PadHandbrake = 0x508
    PadSteer = 0x50C
    RPMMax = 0x3E0
    RearRightWheelOffset = 0x3FC
    SteadyParams = 0x280
    SteerLeft = 0x513
    SteerRight = 0x514
    SystemDataContainer = 0x4D8
    UseKeyboard = 0x515
    VehicleExhaust = 0x330
    VehicleExhaustParticle = 0x438
    VehicleState = 0x1E0
    WheelDust = 0x300
    WheelDustParticle = 0x428
    WheelSlideSound = 0x378
    WheelSlideSoundVolume = 0x3F0
    WheelSlipRatioThreshold = 0x3D0
    WheelSuspensionCompressedThreshold = 0x3D8
    WheelSuspensionElongatedThreshold = 0x3D4
    WheelSuspensionSound = 0x390
    WheelSuspensionSoundVolume = 0x3F4
    WheelTrail = 0x318
    WheelTrailParticle = 0x430

class VehicleConfig:
    BalanceParams = 0xD4
    Chassis = 0xC8
    ChassisMaterials = 0x80
    ChassisShapes = 0xB0
    CollisionInfo = 0xD0
    Drive = 0x58
    InAirControllParams = 0xF8
    KeySmooth = 0x40
    PadSmooth = 0x48
    SteerRatio = 0x50
    SuspStickParams = 0xF0
    WheelAndSuspension = 0x60
    WheelMaterials = 0x68
    WheelShapes = 0x98

class VehicleConstraintData:
    CMassRotation = 0x48
    StickyTireForwardData = 0x18
    StickyTireSideData = 0x30

class VehicleController:
    EnableAutoReverse = 0x19
    PadBrake = 0xC
    PadHandbrake = 0x10
    PadSteer = 0x14
    UseKey = 0x18

class VehicleDesc:
    Chassis = 0x120
    ChassisMaterials = 0x2B0
    ChassisShapes = 0x2E0
    CollisionInfo = 0x2F8
    InAirControllParams = 0x320
    KeySmooth = 0x230
    MotorBikeBalanceParams = 0x2FC
    PadSmooth = 0x258
    SteerRatio = 0x280
    SupsStickGroundParams = 0x318
    WheelAndSuspension = 0x148
    WheelMaterials = 0x298
    WheelShapes = 0x2C8

class VehicleDifferential:
    CentreBias = 0x4C
    DiffType = 0x58
    FrontBias = 0x50
    FrontLeftRightSplit = 0x44
    FrontRearSplit = 0x40
    RearBias = 0x54
    RearLeftRightSplit = 0x48

class VehicleDifferential4WData:
    CentreBias = 0xC
    FrontBias = 0x10
    RearBias = 0x14
    Type = 0x18

class VehicleDifferentialDesc:
    CentreBias = 0xC
    FrontBias = 0x10
    RearBias = 0x14

class VehicleDrive:
    AutoBox = 0x58
    Clutch = 0x60
    Differential = 0x40
    DriveEngine = 0x48
    Gear = 0x50
    Steer = 0x68

class VehicleDriveData:
    Ackermann = 0x110
    AutoBox = 0x90
    Clutch = 0x104
    Differential = 0x30
    Gear = 0x4C

class VehicleDriveDesc:
    AutoBox = 0x94
    Clutch = 0x108
    Engine = 0x20
    Gear = 0x50
    Steer = 0x10C

class VehicleDriveDynamicData:
    AutoBoxSwitchTime = 0x50
    CurrentGear = 0x40
    EngineSpeed = 0x48
    GearDownPressed = 0x56
    GearSwitchTime = 0x4C
    GearUpPressed = 0x55
    TargetGear = 0x44
    UseAutoGears = 0x54

class VehicleDynamicState:
    DestVehicleForwardSpeed = 0x10
    EngineSpeed = 0x18
    EngineSpeedDiff = 0x1C
    ForwardSpeedSmoothSpeed = 0x14
    InputAccel = 0x20
    InputBrake = 0x24
    InputHandBrake = 0x28
    InputSteerLeft = 0x2C
    InputSteerRight = 0x30
    VehicleForwardSpeed = 0xC
    Wheels = 0x38

class VehicleDynamicStateWrapper:
    CurrentGear = 0x44
    DestVehicleForwardSpeed = 0x50
    EngineSpeed = 0x58
    EngineSpeedDiff = 0x5C
    ForwardSpeedSmoothSpeed = 0x54
    GarDownPressed = 0x42
    GearUpPressed = 0x41
    InputAccel = 0x60
    InputBrake = 0x64
    InputHandBrake = 0x68
    InputSteerLeft = 0x6C
    InputSteerRight = 0x70
    TargetGear = 0x48
    UseAutoGears = 0x40
    VehicleForwardSpeed = 0x4C
    Wheels = 0x78

class VehicleEngine:
    DampingRateFullThrottle = 0x4C
    DampingRateZeroThrottleClutchDisengaged = 0x54
    DampingRateZeroThrottleClutchEngaged = 0x50
    MOI = 0x40
    MaxOmega = 0x48
    PeakTorque = 0x44
    TorqueCurveEngineSpeed = 0x58
    TorqueCurveEngineTorque = 0x70

class VehicleEngineData:
    DampingRateFullThrottle = 0xC
    DampingRateZeroThrottleClutchDisengaged = 0x14
    DampingRateZeroThrottleClutchEngaged = 0x10
    TorqueCurve = 0x18

class VehicleEngineDesc:
    DampingRateFullThrottle = 0xC
    DampingRateZeroThrottleClutchDisengaged = 0x14
    DampingRateZeroThrottleClutchEngaged = 0x10

class VehicleEulerAngleStabilizerData:
    ActiveMode = 0x21
    AngleIndex = 0x20
    DampingRatio = 0xC
    Enabled = 0x22
    HighActiveAngle = 0x18
    LowActiveAngle = 0x14
    MaxAngAcc = 0x1C
    NaturalFrequency = 0x10

class VehicleExternalForceData:
    Enabled = 0x19
    InLocalFrame = 0x1A
    Mode = 0x18
    Torque = 0xC

class VehicleFlyPlatformTemplate:
    EulerAngleControlDampingRatio = 0x30
    EulerAngleControlFrequence = 0x40
    EulerAngleStableDamperThreshold = 0x38
    EulerAngleStableDampingRatio = 0x2C
    EulerAngleStableFrequence = 0x3C
    EulerAngleStableStrength = 0x34
    MaxEulerAngAcc = 0x28
    MaxSideAcc = 0x20
    MaxSteerSpeed = 0x24
    PitchRange = 0x48
    RBGuid = 0x10
    SideDampRatio = 0x44
    SteerRoll = 0x4C

class VehicleFlySteerYawData:
    Enabled = 0x14
    SideDampRatio = 0x10
    Steer = 0xC

class VehicleGear:
    FinalRatio = 0x40
    Ratios = 0x48
    SwitchTime = 0x44

class VehicleGearData:
    Ratios = 0xC

class VehicleGearDesc:
    Ratios = 0xC

class VehicleInAirControllData:
    EnableAccelControllPitch = 0x19
    EnablePitchControll = 0x18
    EnablePitchTurn = 0x1A
    PitchControllDelayTime = 0x14
    PitchTurnAngleThreshold = 0xC
    PitchTurnSpeed = 0x10

class VehicleInAirControllParams:
    EnableAccelControllPitch = 0x19
    EnablePitchControll = 0x18
    EnablePitchTurn = 0x1A
    PitchControllDelayTime = 0x14
    PitchTurnAngleThreshold = 0xC
    PitchTurnSpeed = 0x10

class VehicleInAirControllParamsWrapper:
    EnableAccelControllPitch = 0x6D46046000
    EnablePitchControll = 0x6D46046000
    EnablePitchTurn = 0x6D46046000
    PitchControllAccel = 0x6D46046000
    PitchControllDelayTime = 0x6D46046000
    PitchControllSpeed = 0x6D46046000
    PitchTurnAngleThreshold = 0x6D46046000
    PitchTurnSpeed = 0x6D46046000
    RollThreshold = 0x6D46046000

class VehicleInAirControllState:
    IsActivated = 0x10
    IsPitchTurnActivated = 0x11
    PitchTurnStartYaw = 0xC

class VehicleKeySmooth:
    AccelFallRate = 0x44
    AccelRiseRate = 0x40
    BrakeFallRate = 0x4C
    BrakeRiseRate = 0x48
    HandBrakeFallRate = 0x54
    HandBrakeRiseRate = 0x50
    SteerLeftFallRate = 0x5C
    SteerLeftRiseRate = 0x58
    SteerRightFallRate = 0x64
    SteerRightRiseRate = 0x60

class VehicleKeySmoothData:
    BrakeFallRate = 0xC
    HandBrakeFallRate = 0x14
    HandBrakeRiseRate = 0x10
    SteerLeftFallRate = 0x1C
    SteerLeftRiseRate = 0x18
    SteerRightFallRate = 0x24
    SteerRightRiseRate = 0x20

class VehicleKeySmoothDesc:
    BrakeFallRate = 0xC
    HandBrakeFallRate = 0x14
    HandBrakeRiseRate = 0x10
    SteerLeftFallRate = 0x1C
    SteerLeftRiseRate = 0x18
    SteerRightFallRate = 0x24
    SteerRightRiseRate = 0x20

class VehicleOrientationAssistorData:
    PitchRotDelayTime = 0x28
    PitchRotEnabled = 0x30
    PitchRotFallRate = 0x20
    PitchRotInput = 0x10
    PitchRotInputByAccel = 0x32
    PitchRotRiseRate = 0x18
    YawRotDelayTime = 0x2C
    YawRotEnabled = 0x31
    YawRotFallRate = 0x24
    YawRotInput = 0x14
    YawRotInputBySteer = 0x33
    YawRotMaxAcc = 0xC
    YawRotRiseRate = 0x1C

class VehicleOrientationAssistorParams:
    PitchRotDelayTime = 0x28
    PitchRotEnabled = 0x30
    PitchRotFallRate = 0x20
    PitchRotInput = 0x10
    PitchRotInputByAccel = 0x32
    PitchRotRiseRate = 0x18
    YawRotDelayTime = 0x2C
    YawRotEnabled = 0x31
    YawRotFallRate = 0x24
    YawRotInput = 0x14
    YawRotInputBySteer = 0x33
    YawRotMaxAcc = 0xC
    YawRotRiseRate = 0x1C

class VehicleOrientationAssistorParamsWrapper:
    PitchRotDelayTime = 0x6D46046000
    PitchRotEnabled = 0x6D46046000
    PitchRotFallRate = 0x6D46046000
    PitchRotInput = 0x6D46046000
    PitchRotInputByAccel = 0x6D46046000
    PitchRotMaxAcc = 0x6D46046000
    PitchRotRiseRate = 0x6D46046000
    PitchRotSpeed = 0x6D46046000
    YawRotDelayTime = 0x6D46046000
    YawRotEnabled = 0x6D46046000
    YawRotFallRate = 0x6D46046000
    YawRotInput = 0x6D46046000
    YawRotInputBySteer = 0x6D46046000
    YawRotMaxAcc = 0x6D46046000
    YawRotRiseRate = 0x6D46046000
    YawRotSpeed = 0x6D46046000

class VehicleOrientationAssistorState:
    PitchRotActivated = 0x28
    PitchRotDelayedTime = 0x20
    YawRotActivated = 0x29
    YawRotDelayedTime = 0x24
    YawRotTorqueAcc = 0x14

class VehiclePadSmooth:
    AccelFallRate = 0x44
    AccelRiseRate = 0x40
    BrakeFallRate = 0x4C
    BrakeRiseRate = 0x48
    HandBrakeFallRate = 0x54
    HandBrakeRiseRate = 0x50
    SteerLeftFallRate = 0x5C
    SteerLeftRiseRate = 0x58
    SteerRightFallRate = 0x64
    SteerRightRiseRate = 0x60

class VehiclePadSmoothData:
    BrakeFallRate = 0xC
    HandBrakeFallRate = 0x14
    HandBrakeRiseRate = 0x10
    SteerLeftFallRate = 0x1C
    SteerLeftRiseRate = 0x18
    SteerRightFallRate = 0x24
    SteerRightRiseRate = 0x20

class VehiclePadSmoothDesc:
    BrakeFallRate = 0xC
    HandBrakeFallRate = 0x14
    HandBrakeRiseRate = 0x10
    SteerLeftFallRate = 0x1C
    SteerLeftRiseRate = 0x18
    SteerRightFallRate = 0x24
    SteerRightRiseRate = 0x20

class VehicleShapeData:
    ShapeCollProps = 0x28
    UseShapeCollProps = 0x4C

class VehicleStableRotateData:
    AngleIndex = 0x10
    Enabled = 0x11
    MaxAngleAcc = 0xC

class VehicleStateFilter:
    ApplyMotion = 0x80
    DTimeFactorDownT = 0x88
    DTimeFactorKeepT = 0x8C
    DTimeFactorStep = 0x90
    DTimeFactorUpT = 0x84
    DebugOutput = 0x81
    DriveRB = 0x130
    TimeErrorTH = 0x94
    UseTaskGraph = 0xF8
    World = 0x100

class VehicleStateFilterEx:
    DebugOutput = 0x9D
    EnableSend = 0x8C
    EnableSendSnapshot = 0x8D
    InterpMethod = 0x94
    LocalMode = 0x80
    MaxBlendTime = 0x98
    SendInterval = 0x84
    SendSnapshotInterval = 0x88
    SnapshotVersion = 0x90
    SyncSnapshot = 0x9C
    UseClientPhysxVehicleSimulate = 0x9E

class VehicleStateFilterUs:
    ApplyMotion = 0x80
    DebugOutput = 0x81

class VehicleStateSender:
    DebugOutput = 0x8D
    EnableSend = 0x8C
    SendInterval = 0x88

class VehicleSteadyParamsWrapper:
    MaxRollVel = 0x6D46046000
    RollSteadyActiveThreshold = 0x6D46046000
    TargetRollAngleVsSteerRatio = 0x6D46046000

class VehicleSteerRatio:
    Speed = 0x40
    SteerRatio = 0x58

class VehicleStickyTireConstraintData:
    ActiveFlag = 0x1C
    Dir = 0xC
    TargetSpeed = 0x18

class VehicleSurfaceDisturbData:
    Enable = 0xC

class VehicleSurfaceDisturbSimData:
    Distance = 0x40
    IsValid = 0x44

class VehicleSurfaceToTireFrictionPairsConfig:
    DefaultFriction = 0x48
    SurfaceTypeCnt = 0x44
    TireTypeCnt = 0x40

class VehicleSuspLimitConstraintData:
    ActiveFlag = 0x1C
    Dir = 0xC
    Error = 0x18

class VehicleSuspension:
    CamberAtMaxCompression = 0x58
    CamberAtMaxDroop = 0x5C
    CamberAtRest = 0x54
    MaxCompression = 0x48
    MaxDroop = 0x4C
    SpringDamperRate = 0x44
    SpringStrength = 0x40
    SprungMass = 0x50

class VehicleSuspensionData:
    CamberAtMaxCompression = 0x18
    CamberAtMaxDroop = 0x1C
    CamberAtRest = 0x14
    ForceApplyPointOffset = 0x2C
    MaxDroop = 0xC
    SprungMass = 0x10
    TravelDirection = 0x20

class VehicleSuspensionDesc:
    CamberAtMaxCompression = 0x18
    CamberAtMaxDroop = 0x1C
    CamberAtRest = 0x14
    MaxDroop = 0xC
    SprungMass = 0x10

class VehicleSystemComponent:
    DebugDraw = 0x158
    EnableInterpolatePose = 0x201
    SystemDataContainer = 0x1C8
    TemplateRes = 0xF8
    VehicleChassisRB = 0x1A8
    VehicleController = 0x150

class VehicleSystemDataWrapperContainer:
    DataWrappers = 0x40

class VehicleSystemDebugDrawComponent:
    CMLocalFrameScale = 0x84
    FrameScale = 0x80
    ShowCMLocalFrame = 0x8E
    ShowFrame = 0x8D
    ShowSuspOrigData = 0x8B
    ShowSuspensions = 0x8A
    ShowTrailerLinks = 0x8C
    ShowVehicleRB = 0x88
    ShowVehicleRBInterpolatedPose = 0x89
    SuspensionsColor = 0x70
    VehicleRBColor = 0x60

class VehicleSystemTemplateData:
    AntiGravitySuspensions = 0x20
    SuspensionQueryFilterData = 0x38
    VehicleRBGuid = 0x10

class VehicleThrustVectorData:
    Accel = 0x14
    BackwardSpeed = 0x10
    Enabled = 0x3C
    ForwardSpeed = 0xC
    SideAcc = 0x18
    SideAccel = 0x20
    SideSpeed = 0x1C
    Steer = 0x30
    SteerDownAcc = 0x28
    SteerSpeed = 0x2C
    SteerUpAcc = 0x24
    WindDragMinAcc = 0x38
    WindDragRatio = 0x34

class VehicleTire:
    CamberStiffnessPerUnitGravity = 0x4C
    FrictionVsSlip00 = 0x50
    FrictionVsSlip01 = 0x54
    FrictionVsSlip10 = 0x58
    FrictionVsSlip11 = 0x5C
    FrictionVsSlip20 = 0x60
    FrictionVsSlip21 = 0x64
    LatStiffX = 0x40
    LatStiffY = 0x44
    LongitudinalStiffnessPerUnitGravity = 0x48
    TireType = 0x68

class VehicleTireData:
    CamberStiffnessPerUnitGravity = 0xC
    FrictionVsSlip = 0x14
    TireForceApplyCMOffsets = 0x2C
    Type = 0x10

class VehicleTireDesc:
    CamberStiffnessPerUnitGravity = 0xC
    Type = 0x10

class VehicleTireLoadFilter:
    MaxFilteredNormalisedLoad = 0x4C
    MaxNormalisedLoad = 0x48
    MinFilteredNormalisedLoad = 0x44
    MinNormalisedLoad = 0x40

class VehicleTireLoadFilterData:
    MaxFilteredNormalisedLoad = 0xC

class VehicleTireLoadFilterDesc:
    MaxFilteredNormalisedLoad = 0xC

class VehicleTireSurfaceConfig:
    TireSurfaceFrictions = 0x20

class VehicleTrailerBody:
    Link = 0x2C
    OffsetPose = 0x10
    WheelSimData = 0x68

class VehicleTrailerLink:
    Ackermann = 0x28
    AnchorOnChildF = 0xC
    Angle0Limit = 0x18
    Angle1Limit = 0x1C
    Angle2Limit = 0x20
    EnableBrake = 0x24
    EnableSteer = 0x25

class VehicleTrailerTemplate:
    TrailerHead = 0x10
    Trailers = 0x290

class VehicleUGCTemplate:
    RBs = 0x10

class VehicleWheel:
    DampingRate = 0x50
    MOI = 0x4C
    Mass = 0x48
    MaxBrakeTorque = 0x54
    MaxHandBrakeTorque = 0x58
    MaxSteer = 0x5C
    Radius = 0x40
    ToeAngle = 0x60
    Width = 0x44

class VehicleWheelData:
    CenterOffset = 0x24
    DampingRate = 0x10
    MOI = 0xC
    MaxBrakeTorque = 0x14
    MaxHandBrakeTorque = 0x18
    MaxSteer = 0x1C
    ToeAngle = 0x20

class VehicleWheelDesc:
    DampingRate = 0x10
    MOI = 0xC
    MaxBrakeTorque = 0x14
    MaxHandBrakeTorque = 0x18
    MaxSteer = 0x1C
    ToeAngle = 0x20

class VehicleWheelDynamicData:
    Jounce = 0x14
    TireLowForwardSpeedTimer = 0xC
    TireLowSideSpeedTimer = 0x10

class VehicleWheelDynamicState:
    CamberAngle = 0x4C
    LateralSlip = 0x44
    LocalPose = 0x50
    LongitudinalSlip = 0x40
    SteerAngle = 0x48
    SuspJounce = 0x20
    SuspSpringForce = 0x24
    TireContactNormal = 0x10
    TireFriction = 0x1C
    TireLateralDir = 0x34
    TireLongitudinalDir = 0x28
    WheelRotationAngle = 0x70
    WheelSpeed = 0x6C

class VehicleWheelDynamicStateWrapper:
    CamberAngle = 0x98
    IsInAir = 0x40
    LateralSlip = 0x90
    LocalPose = 0x9C
    LongitudinalSlip = 0x8C
    SteerAngle = 0x94
    SuspJounce = 0x6C
    SuspSpringForce = 0x70
    TireContactNormal = 0x5C
    TireContactPoint = 0x50
    TireFriction = 0x68
    TireLateralDir = 0x80
    TireLongitudinalDir = 0x74
    WheelRotationAngle = 0xBC
    WheelSpeed = 0xB8

class VehicleWheelSuspData:
    Enabled = 0xA4
    MapShapeId = 0xA0
    TireData = 0x68
    WheelData = 0x38

class VehicleWheelSuspension:
    AntiRollBars = 0xE8
    SuspForceAppCMOffsets = 0xB8
    SuspTravelDirections = 0x88
    Suspensions = 0x70
    TireForceAppCMOffsets = 0xD0
    TireLoadFilter = 0x100
    Tires = 0x58
    WheelCentreCMOffsets = 0xA0
    WheelEnableFlags = 0x108
    Wheels = 0x40

class VehicleWheelSuspensionDesc:
    AntiRollBars = 0xA8
    SuspForceAppCMOffsets = 0x78
    SuspTravelDirections = 0x48
    Suspensions = 0x30
    TireForceAppCMOffsets = 0x90
    TireLoadFilter = 0xC0
    Tires = 0x18
    WheelCentreCMOffsets = 0x60

class VehicleWheelsSimData:
    AntiRollBars = 0x18
    HighForwardSpeedSubStepCount = 0x50
    LowForwardSpeedSubStepCount = 0x4C
    MinLongSlipDenominator = 0x54
    SupsStickDatas = 0x30
    ThresholdLongitudinalSpeed = 0x48
    TireLoadFilter = 0x38
    UseSweepQuery = 0x58

class VirtualCamera:
    BlendRatio = 0x94
    EnableDebug = 0xB0
    IsPostModify = 0x88
    Reversed = 0x90
    VCName = 0x98
    VCPriority = 0x8C

class VirtualCameraController:
    ControlledCamera = 0x70
    Controllers = 0x78

class VirtualCameraDebugger:
    ShowCamera = 0x80
    ShowFrustum = 0x81
    ShowName = 0x82

class VirtualCameraManager:
    BlendStack = 0x3F8
    ControlledCamera = 0x2F8
    Controllers = 0x3E0
    Debugger = 0x3D8
    Enable = 0x3D0
    ModifyStack = 0x410

class VirtualFollowCamera:
    AutoUpdate = 0xE8
    FollowRotation = 0xF8
    FollowTarget = 0xE0
    GlobalOffset = 0xFC
    LocalOffset = 0x12C

class VirtualLookAtCamera:
    AutoUpdate = 0xE8
    LookAtTarget = 0xE0
    Offset = 0xF8

class VisibilityCubeComponent:
    Enable = 0x10E
    Resource = 0x110

class VisualGraphComponent:
    HoldResource = 0x2BD
    IsActive = 0x2BE
    IsStopped = 0x2BF

class VisualGraphElementBurstSource:
    BurstCount = 0x44
    Delay = 0x4C
    Duration = 0x58
    Period = 0x54

class VisualGraphElementConstantSource:
    Duration = 0x4C
    SpawnRate = 0x44
    SpawnRateParameterIndex = 0x48

class VisualGraphElementCustomMeshData:
    DepthTest = 0xD1
    DepthWrite = 0xD2
    Mesh = 0xC0
    Solid = 0xD0

class VisualGraphElementCustomRenderData:
    CurveParameters = 0x80
    OverrideParameters = 0xA8
    Parameters = 0x68
    Shader = 0x48

class VisualGraphElementCustomSpriteData:
    DepthTest = 0xC0
    DepthWrite = 0xC1

class VisualGraphElementMeshData:
    DepthTest = 0x69
    DepthWrite = 0x6A
    Diffuse = 0x58
    Mesh = 0x48
    Solid = 0x68

class VisualGraphElementSpriteData:
    DepthTest = 0x58
    DepthWrite = 0x59
    Texture = 0x48

class VisualGraphEmitter:
    AliveOffset = 0x56
    BaseDepth = 0x5A
    BufferCount = 0x58
    EmitterId = 0x50
    ExternalBufferData = 0x68
    IsTriggered = 0x81
    Local = 0x80
    MaxFadeTime = 0x4C
    MaxParticle = 0x48
    PostBaseDepth = 0x5C
    PostUpdateDepth = 0x60
    PropertyStride = 0x54
    RenderDatas = 0xD0
    RenderTasks = 0xE8
    SpawnIndex = 0x52
    StartNode = 0xA8
    TriggerSource = 0x88
    UpdateDepth = 0x5E
    UpdateTaskTree = 0xB8

class VisualGraphExposedParameter:
    Value = 0x20

class VisualGraphExtBufferData:
    PrimitiveIndex = 0x40
    SlotMask = 0x3C
    Stride = 0x38
    TextureArrayDepth = 0x41
    TextureArrayLayout = 0x48
    Type = 0x18
    Value = 0x20

class VisualGraphGroup:
    Elements = 0x40
    Parameters = 0x58
    Precedence = 0x70

class VisualGraphSystem:
    BoundingBox = 0x58
    Emitters = 0xB8
    ExposedParameters = 0x88
    Shader = 0x70
    SpawnGroups = 0xA0

class VisualGraphTaskNode:
    CurveParameters = 0x80
    Depth = 0x16
    DstBuffer = 0x14
    ExposedParameterIndices = 0x68
    MaxTriggerNum = 0x1C
    Parameters = 0x50
    PostDepth = 0x18
    PropertyParameters = 0xA8
    ShaderExt = 0x20
    SrcBuffer = 0x12
    Technique = 0x38
    Type = 0x10

class VisualGraphTriggerInfo:
    SrcTask = 0x10

class VolumeComponent:
    ControlPoints = 0x180
    IsInVolume = 0x149
    IsInVolumeBound = 0x148
    LocalBound = 0x14C
    ShapeType = 0x17C
    World = 0x140
    WorldBound = 0x164

class WFDebugDrawer:
    BottomScale = 0x7C
    Color = 0x64
    ShowFieldIndicator = 0x60
    ShowFieldSlice = 0x80
    SliceLevel = 0x84
    VelAtMaxLength = 0x78
    VelAtMinLength = 0x74

class WaterWaveComponent:
    ChunkRes = 0x6E8
    Collected = 0x75C
    CullingBound = 0x718
    CullingFlags = 0x700
    DisplaceRange = 0x774
    EnableSSR = 0x6FC
    FocusPosition = 0x768
    FrameIndex = 0x764
    FrameTime = 0x760
    GridRes = 0x6F4
    HeightRange = 0x784
    IsFoamAlphaReady = 0x759
    IsFoamNormalReady = 0x75A
    IsMaterialReady = 0x751
    IsShorelineReady = 0x75B
    IsTextureReady = 0x758
    IsWaterWaveResourceReady = 0x750
    LodRangeScale = 0x6F8
    MaterialGuid = 0x7A8
    NeedUpdateMesh = 0x730
    NoFarPlaneClip = 0x6E4
    RequiredTerrains = 0x790
    SectorRes = 0x6EC
    ShorelineBoundInfo = 0x7D8
    ShorelineGuid = 0x7C8
    ShorelineRangeInfo = 0x7E8
    ShorelineWaveFoamGuid = 0x7F8
    ShorelineWaveNoiseGuid = 0x808
    Size = 0x6F0
    TexturesReadyCount = 0x754
    Viewpoints = 0x738
    WaterWaveGuid = 0x7B8

class WaterWaveInfo:
    Amplitude = 0x24
    ChoppyScale = 0x34
    DirBias = 0x40
    DirMovement = 0x44
    DisplaceRanges = 0x98
    DisplacementMaps = 0xB0
    FoamAlphaGuid = 0x70
    FoamAmount = 0x48
    FoamDissipationSpeed = 0x50
    FoamFallOffPower = 0x58
    FoamFallOffSpeed = 0x54
    FoamNormalGuid = 0x80
    FoamThreshold = 0x4C
    FrameCount = 0x60
    FrameIntervalTime = 0x64
    FrameIntervalTimeInv = 0x68
    GridSize = 0x20
    Length = 0x1C
    Noise = 0x3C
    NormalMaps = 0xF8
    PeriodCount = 0x5C
    PlayPeriod = 0x90
    SlopeRanges = 0xE0
    SmallCutOff = 0x28
    UVScale = 0x6C
    UnitPeriod = 0x38
    WindAngle = 0x30
    WindStrength = 0x2C

class WheelSuspensionConstraintData:
    DriveAngularSpeed = 0xCC
    DriveDamping = 0xC8
    DriveSpring = 0xC4
    EnableDrive = 0xD0
    SuspensionDamping = 0xC0
    SuspensionExtent = 0xB8
    SuspensionSpring = 0xBC

class WindFieldComponent:
    AdvectionBlendParam = 0x314
    AmbientOnly = 0x324
    AmbientWind = 0x2A4
    DebugDrawer = 0x298
    DeltaTime = 0x30C
    DiffusionRate = 0x310
    EnableAdvection = 0x320
    EnableDiffusion = 0x321
    EnableNoiseAmbient = 0x323
    EnableProjection = 0x322
    FieldSize = 0x2E0
    FollowEntity = 0x2A1
    GridLength = 0x2EC
    Is3D = 0x325
    NBDiffusionIter = 0x318
    NBProjectIter = 0x31C
    NoiseDisturbance = 0x2D8
    NoiseTexId = 0x2B0
    NoiseTexToWorld = 0x2C0
    NoiseUVResetTime = 0x2DC
    NoiseUVSize = 0x2CC
    PositionOffset = 0x2F0

class WindFieldData:
    AdvectionBlendParam = 0x80
    AmbientNoiseTexId = 0x40
    AmbientTexToWorldMap = 0x24
    AmbientUVSize = 0x30
    AmbientWind = 0x18
    AmbientWindDisturbance = 0x50
    DeltaTime = 0x78
    DiffusionRate = 0x7C
    EnableAdvection = 0x8C
    EnableDiffusion = 0x8D
    EnableNoiseAmbient = 0x8F
    EnableProjection = 0x8E
    FieldSize = 0x68
    GridLength = 0x74
    MinPos = 0x5C
    NBDiffusionIter = 0x84
    NBProjectIter = 0x88
    NoiseSampleLevel = 0x54
    NoiseUVResetTime = 0x58
    constantWind = 0xC

class WindMotor:
    direction = 0x18
    otorType = 0x2C
    position = 0xC
    radius = 0x28
    strength = 0x24

class X9:
    42 DH = 0x10

class YaFpsMover:
    EscapedTime = 0x164
    MoveTime = 0x160
    PitchMax = 0x174
    PitchMin = 0x170

class YaFpsPlacer:
    BasePosition = 0xC4
    BoneName = 0x90
    Direction = 0xDC
    Fov = 0xC0
    Mover = 0xB0
    PitchMax = 0xBC
    PitchMin = 0xB8
    PositionOffset = 0xD0
    Target = 0xE8
    UseBoneDirection = 0x89
    UseBonePosition = 0x88
    UseBoneRoll = 0x8A

class YaPivotPlacer:
    Aspect = 0xD4
    Direction = 0xF8
    Finished = 0xC0
    FocusPosOffset = 0xE0
    FocusRatio = 0xB8
    Fov = 0xD0
    MajorTarget = 0x138
    MajorTargetPos = 0x11C
    MajorTargetPosOffset = 0x104
    MajorTargetTransform = 0x148
    MarginPercent = 0xC4
    MaxDist = 0xCC
    MinDist = 0xC8
    MinorTarget = 0x140
    MinorTargetPos = 0x128
    MinorTargetPosOffset = 0x110
    MinorTargetTransform = 0x178
    PitchAngle = 0xD8
    PivotRatio = 0xDC
    RadialRatio = 0xBC
    RangeMaxPoint = 0xAC
    RangeMinPoint = 0xA0
    ReferDirection = 0xEC

class YawMover:
    AngularAccelerateTime = 0x174
    AngularAcceleration = 0x170
    AngularRatio = 0x18C
    AngularSpeed = 0x17C
    AngularTolerance = 0x19C
    BrakeAngle = 0x178
    Clockwise = 0x16D
    EscapedTime = 0x160
    FocusRatio = 0x190
    FocusSpeed = 0x180
    FocusTolerance = 0x1A0
    FovRatio = 0x164
    FovTolerance = 0x168
    LerpAsBrake = 0x16C
    RadialRatio = 0x194
    RadialSpeed = 0x184
    RadialTolerance = 0x1A4
    VerticalRatio = 0x198
    VerticalSpeed = 0x188
    VerticalTolerance = 0x1A8

class _abc:
    _abc_data = 0x4000

class _blake2:
    blake2b = 0x100
    blake2s = 0x100

class _bz2:
    BZ2Compressor = 0x100
    BZ2Decompressor = 0x100

class _csv:
    Dialect = 0x4500
    Error = 0x400
    reader = 0x4500
    writer = 0x4500

class _hashlib:
    HASH = 0x580
    HASHXOF = 0x580
    HMAC = 0x180

class _json:
    Encoder = 0x4000
    Scanner = 0x4000

class _lsprof:
    Profiler = 0x4500

class _lzma:
    LZMACompressor = 0x100
    LZMADecompressor = 0x100

class _md5:
    md5 = 0x4180

class _multibytecodec:
    MultibyteCodec = 0x4180
    MultibyteIncrementalDecoder = 0x4500
    MultibyteIncrementalEncoder = 0x4500
    MultibyteStreamReader = 0x4500
    MultibyteStreamWriter = 0x4500

class _pickle:
    Pdata = 0x8

class _random:
    Random = 0x400

class _sha1:
    sha1 = 0x4180

class _sha256:
    sha224 = 0x4180
    sha256 = 0x4180

class _sha3:
    sha3_224 = 0x100
    sha3_256 = 0x100
    sha3_384 = 0x100
    sha3_512 = 0x100
    shake_128 = 0x100
    shake_256 = 0x100

class _sha512:
    sha384 = 0x4180
    sha512 = 0x4180

class _sre:
    SRE_Scanner = 0x4180

class _ssl:
    Certificate = 0x180
    MemoryBIO = 0x4100
    SSLSession = 0x4180
    _SSLContext = 0x4500
    _SSLSocket = 0x4100

class _struct:
    Struct = 0x4500
    unpack_iterator = 0x4100

class _thread:
    RLock = 0x4500
    _local = 0x4500
    _localdummy = 0x180
    lock = 0x4180

class array:
    array = 0x4520
    arrayiterator = 0x4180

class array(typecode [, initializer]) -> array

Return a new array whose items are restricted by typecode, and
initialized from the optional initializer value, which must be a list,
string or iterable over elements of the appropriate type:
    Arrays represent basic values and behave very much like lists, except
the type of objects stored in them is constrained. The type is specified
at object creation time by using a type code, which is a single character.
The following type codes are defined:

    Type code   C Type             Minimum size in bytes
    'b'         signed integer     1
    'B'         unsigned integer   1
    'u'         Unicode character  2 (see note)
    'h'         signed integer     2
    'H'         unsigned integer   2
    'i'         signed integer     2
    'I'         unsigned integer   2
    'l'         signed integer     4
    'L'         unsigned integer   4
    'q'         signed integer     8 (see note)
    'Q'         unsigned integer   8 (see note)
    'f'         floating point     4
    'd'         floating point     8

NOTE: The 'u' typecode corresponds to Python's unicode character. On
narrow builds this is 2-bytes on wide builds this is 4-bytes.

NOTE: The 'q' and 'Q' type codes are only available if the platform
C compiler used to build Python supports 'long long', or, on Windows,
'__int64'.

Methods:

append() -- append a new item to the end of the array
buffer_info() -- return information giving the current memory info
byteswap() -- byteswap all the items of the array
count() -- return number of occurrences of an object
extend() -- extend array by appending multiple elements from an iterable
fromfile() -- read items from a file object
fromlist() -- append items from the list
frombytes() -- append items from the string
index() -- return index of first occurrence of an object
insert() -- insert a new item into the array at a provided position
pop() -- remove and return item (default last)
remove() -- remove first occurrence of an object
reverse() -- reverse the order of the items in the array
tofile() -- write all items to a file object
tolist() -- return the array converted to an ordinary list
tobytes() -- return the array converted to a string

Attributes:

typecode -- the typecode character used to create the array
itemsize -- the length in bytes of one array item = 0x10

class ast:
    AST = 0x4400

class asyncgen_hooks

A named tuple providing information about asynchronous
generators hooks:
    The attributes are read only. = 0x2

class attrgetter(attr,:
    ..) --> attrgetter object

Return a callable object that fetches the given attribute(s) from its operand.
After f = attrgetter('name'), the call f(r) returns r.name.
After g = attrgetter('name', 'date'), the call g(r) returns (r.name, r.date).
After h = attrgetter('name.first', 'name.last'), the call h(r) returns
(r.name.first, r.name.last). = 0x10

class bool (Messiah:
    IObject.*)(const std.string &, const Messiah.Color4 &, bool) noexcept>.__name() [_type = bool (Messiah.IObject.*)(const std.string &, const Messiah.Color4 &, bool) noexcept] = 0x70
    IObject.*)(std.string, bool) noexcept>.__name() [_type = bool (Messiah.IObject.*)(std.string, bool) noexcept] = 0x10

class d:
    cpsuri = 0xA5
    dNSName = 0x88
    directoryName = 0x88
    ediPartyName = 0x88
    iPAddress = 0x88
    other = 0x67
    otherName = 0x88
    rfc822Name = 0x88
    uniformResourceIdentifier = 0x88
    x400Address = 0x90

class datetime:
    IsoCalendarDate = 0x8

class functools:
    KeyWrapper = 0x4180
    _lru_cache_wrapper = 0x24100
    _lru_list_elem = 0x180
    partial = 0x4D00

class hash_info

A named tuple providing parameters used for computing
hashes:
    The attributes are read only. = 0x9

class itemgetter(item,:
    ..) --> itemgetter object

Return a callable object that fetches the given item(s) from its operand.
After f = itemgetter(2), the call f(r) returns r[2].
After g = itemgetter(2, 5, 3), the call g(r) returns (r[2], r[5], r[3]) = 0x10

class mData[0]:
    mCenter = 0x1
    mExtents = 0x1

class mData[1]:
    mCenter = 0x1
    mExtents = 0x1

class mData[2]:
    mCenter = 0x1
    mExtents = 0x1

class methodcaller(name,:
    ..) --> methodcaller object

Return a callable object that calls the given method on its operand.
After f = methodcaller('name'), the call f(r) returns r.name().
After g = methodcaller('name', 'date', foo=1), the call g(r) returns
r.name('date', foo=1). = 0x10

class name:
    fullname = 0x8A

class operator:
    attrgetter = 0x4900
    itemgetter = 0x4900
    methodcaller = 0x4100

class p:
    onBasis = 0x2AA
    other = 0x1
    prime = 0x197
    tpBasis = 0x2AB

class posix:
    DirEntry = 0x80
    ScandirIterator = 0x81

class pyexpat:
    xmlparser = 0x4180

class re:
    Match = 0x4180
    Pattern = 0x4180

class select:
    poll = 0x80

class sqlite3:
    Blob = 0x4180
    Connection = 0x4500
    Cursor = 0x4500
    PrepareProtocol = 0x4100
    Row = 0x4500
    Statement = 0x4180

class ssl:
    SSLError = 0x500

class stat_result: Result from stat, fstat, or lstat:
    This object may be accessed either as a tuple of
  (mode, ino, dev, nlink, uid, gid, size, atime, mtime, ctime)
or via the attributes st_mode, st_ino, st_dev, st_nlink, st_uid, and so on.

Posix/windows: If your platform supports st_blksize, st_blocks, st_rdev,
or st_flags, they are available as attributes only.

See os.stat for more information. = 0xA

class statvfs_result: Result from statvfs or fstatvfs:
    This object may be accessed either as a tuple of
  (bsize, frsize, blocks, bfree, bavail, files, ffree, favail, flag, namemax),
or via the attributes f_bsize, f_frsize, f_blocks, f_bfree, and so on.

See os.statvfs for more information. = 0xA

class sys:
    flags

Flags provided through command line arguments or environment vars. = 0x12
    float_info

A named tuple holding information about the float type. It contains low level
information about the precision and internal representation. Please study
your system's :file:`float.h` for more information. = 0xB
    int_info

A named tuple that holds information about Python's
internal representation of integers.  The attributes are read only. = 0x4
    thread_info

A named tuple holding information about the thread implementation. = 0x3
    version_info

Version information as a named tuple. = 0x5

class times_result: Result from os:
    times().

This object may be accessed either as a tuple of
  (user, system, children_user, children_system, elapsed),
or via the attributes user, system, children_user, children_system,
and elapsed.

See os.times for more information. = 0x5

class type:
    googleapis.com/ = 0x6D4796C774

class u:
    addressRange = 0x2
    addressesOrRanges = 0x1
    asIdsOrRanges = 0x1
    inherit = 0x4
    range = 0x2

class uname_result: Result from os:
    uname().

This object may be accessed either as a tuple of
  (sysname, nodename, release, version, machine),
or via the attributes sysname, nodename, release, version, and machine.

See os.uname for more information. = 0x5

class unicodedata:
    UCD = 0x4180

class value:
    byKey = 0x1
    byName = 0x90
    good = 0x88
    implicitlyCA = 0x1
    revoked = 0x88
    unknown = 0x1

class void (Messiah:
    IEntity *, const Messiah.TMatrix4x3<float> &) noexcept>.__name() [_type = void (Messiah.IEntity *, const Messiah.TMatrix4x3<float> &) noexcept] = 0x60
    INavigateMap *, const Messiah.Guid &) noexcept>.__name() [_type = void (Messiah.INavigateMap *, const Messiah.Guid &) noexcept] = 0x10
    IObject.*)(Messiah.Heightmap *) noexcept>.__name() [_type = void (Messiah.IObject.*)(Messiah.Heightmap *) noexcept] = 0x70
    IObject.*)(Messiah.IComponent *) noexcept>.__name() [_type = void (Messiah.IObject.*)(Messiah.IComponent *) noexcept] = 0x10
    IObject.*)(Messiah.IEnvironmentVolume *) noexcept>.__name() [_type = void (Messiah.IObject.*)(Messiah.IEnvironmentVolume *) noexcept] = 0x10
    IObject.*)(Messiah.IObject *) noexcept>.__name() [_type = void (Messiah.IObject.*)(Messiah.IObject *) noexcept] = 0x10
    IObject.*)(Messiah.IPrimitive *) noexcept>.__name() [_type = void (Messiah.IObject.*)(Messiah.IPrimitive *) noexcept] = 0x6D4B6F4000
    IObject.*)(Messiah.ISpace *) noexcept>.__name() [_type = void (Messiah.IObject.*)(Messiah.ISpace *) noexcept] = 0x10
    IObject.*)(bool, const std.string &) noexcept>.__name() [_type = void (Messiah.IObject.*)(bool, const std.string &) noexcept] = 0x10
    IObject.*)(const Messiah.TVec3<float> &) noexcept>.__name() [_type = void (Messiah.IObject.*)(const Messiah.TVec3<float> &) noexcept] = 0x10
    IObject.*)(const Messiah.TVec3<float> &, float) noexcept>.__name() [_type = void (Messiah.IObject.*)(const Messiah.TVec3<float> &, float) noexcept] = 0x10
    IObject.*)(const std.string &, const std.string &, const std.string &, float, float, unsigned char, unsigned char) noexcept>.__name() [_type = void (Messiah.IObject.*)(const std.string &, const std.string &, const std.string &, float, float, unsigned char, unsigned char) noexcept] = 0x10
    IObject.*)(unsigned char, bool) noexcept>.__name() [_type = void (Messiah.IObject.*)(unsigned char, bool) noexcept] = 0x10
    IPhysicsDynamicLoadBody *) noexcept>.__name() [_type = void (Messiah.IPhysicsDynamicLoadBody *) noexcept] = 0x6D4844CE38
    IStoryboard *, float) noexcept>.__name() [_type = void (Messiah.IStoryboard *, float) noexcept] = 0x10
    Name) noexcept>.__name() [_type = void (Messiah.Name) noexcept] = 0x10
    PhysicsCollidableComponent *) noexcept>.__name() [_type = void (Messiah.PhysicsCollidableComponent *) noexcept] = 0x10
    SceneChunkId, bool) noexcept>.__name() [_type = void (Messiah.SceneChunkId, bool) noexcept] = 0x10

class void (float) noexcept>:
    __name() [_type = void (float) noexcept] = 0x10

class waitid_result: Result from waitid:
    This object may be accessed either as a tuple of
  (si_pid, si_uid, si_signo, si_status, si_code),
or via the attributes si_pid, si_uid, and so on.

See os.waitid for more information. = 0x5

class zlib:
    Compress = 0x80
    Decompress = 0x80

