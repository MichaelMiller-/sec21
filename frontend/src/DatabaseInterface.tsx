// todo; code duplication: see entity-folder from "backend"
// todo: maybe replace 'interface' with a class and default constructors

export interface DbProject {
    id: number;
    name: string;
    title: string;
    createdAt: Date;
    modifiedAt: Date;
}

export interface DbStructuralPoint {
    id: number;
    name: string;
    uuid: string;
    coordinate_x: number;
    coordinate_y: number;
    coordinate_z: number;
}

export interface DbMaterial {
    id: number;
    name: string;
    uuid: string;
    E_modulus: number;
}

export enum SystemLine {
    CENTRE = "centre",
    TOP = "top",
    BOTTON = "buttom",
    LEFT = "left",
    RIGHT = "right",
    TOPLEFT = "top_left",
    TOPRIGHT = "top_right",
    BOTTOMLEFT = "buttom_left",
    BOTTOMRIGHT = "buttom_right"
}

export interface DbCrossSection {
    id: number;
    name: string;
    uuid: string;
    material: DbMaterial;
    area: number;
    systemLine: SystemLine;
}

export interface DbCurveMember {
    id: number;
    name: string;
    uuid: string;
    systemLine: SystemLine;
    beginNode: DbStructuralPoint;
    endNode: DbStructuralPoint;
    crossSection: DbCrossSection;
}

export enum ActionType {
    PERMANENT = "Permanent",
    VARIABLE = "Variable",
    ACCIDENTAL = "Accidental"
}

export enum LoadType {
    SELFWEIGHT = "SelfWeight",
    OTHERS = "Others",
    PRESTRESS = "Prestress",
    DYNAMIC = "Dynamic",
    STATIC = "Static",
    TEMPERATURE = "Temperature",
    WIND = "Wind",
    SNOW = "Snow",
    MAINTENANCE = "Maintenance",
    FIRE = "Fire",
    MOVING = "Moving",
    SEISMIC = "Seismic",
    STANDARD = "Standard"
}

export interface DbLoadCase {
    id: number;
    name: string;
    uuid: string;
    description: string;
    actionType: ActionType;
    type: LoadType;
}

export enum SupportType {
    FIXED = "fixed",
    HINGED = "hinged",
    SLIDING = "sliding",
    CUSTOM = "custom"
}

export interface DbPointSupport {
    id: number;
    name: string;
    uuid: string;
    type: SupportType;
    referencePoint: DbStructuralPoint;
}

export interface DbPointAction {
    id: number;
    name: string;
    uuid: string;
    value_x: number;
    value_y: number;
    value_z: number;
    loadcase: DbLoadCase;
    referencePoint: DbStructuralPoint;
}

export enum ThicknessType {
    CONSTANT = "Constant",
    TOP = "top",
    BOTTOM = "buttom",
    LEFT = "left",
    RIGHT = "right",
    TOPLEFT = "top_left",
    TOPRIGHT = "top_right",
    BOTTOMLEFT = "buttom_left",
    BOTTOMRIGHT = "buttom_right"
}

export enum SystemPlane {
    BOTTOM = "bottom",
    CENTRE = "centre",
    TOP = "top"
}

export enum LCSType {
    X_BY_VECTOR = "x_by_vector",
    Y_BY_VECTOR = "y_by_vector",
    TILT = "tilt_of_vector_defined_by_point"
}

export enum BehaviorAnalysis {
    ISOTROPIC = "isotropic",
    // Orthotropic,
    // Membrane,
    // Press only
}

export interface DbSurfaceMember {
    id: number
    name: string
    uuid: string
    thicknessType: ThicknessType
    thickness: number
    coordinateX: number
    coordinateY: number
    coordinateZ: number
    systemPlane: SystemPlane
    edges: string
    lcsRotation: number
    zAnalysisEccentricity: number
    behaviorAnalysis: BehaviorAnalysis
    lcsType: LCSType
    nodes: DbStructuralPoint[]
}