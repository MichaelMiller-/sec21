import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated, ManyToMany, JoinTable } from "typeorm";
import { Project } from "./Project";
import { StructuralPoint } from "./StructuralPoint";
import { Material } from "./Material";

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

@Entity()
export class SurfaceMember {

    @PrimaryGeneratedColumn()
    id: number;

    @Column()
    name: string;

    @Column()
    @Generated("uuid")
    uuid: string;

    @ManyToOne(type => Material, mat => mat.id)
    material: Material;

    @Column({
        type: "enum",
        enum: ThicknessType,
        default: ThicknessType.CONSTANT
    })
    thicknessType: ThicknessType;

    @Column("real") // [mm]
    thickness: number;

    @Column({
        type: "enum",
        enum: SystemPlane,
        default: SystemPlane.CENTRE
    })
    systemPlane: SystemPlane;

    @Column()
    edges: string = "Line";

    @Column({
        type: "enum",
        enum: LCSType,
        default: LCSType.X_BY_VECTOR
    })
    lcsType: LCSType;

    @Column("real")
    coordinateX: number;

    @Column("real")
    coordinateY: number;

    @Column("real")
    coordinateZ: number;

    @Column("real") // [degree]
    lcsRotation: number;

    @Column("real") // [mm]
    zAnalysisEccentricity: number;

    @Column({
        type: "enum",
        enum: BehaviorAnalysis,
        default: BehaviorAnalysis.ISOTROPIC
    })
    behaviorAnalysis: BehaviorAnalysis;

    @Column("simple-json")
    nodes: StructuralPoint[];

    @ManyToMany(type => StructuralPoint, pt => pt.id)
    @JoinTable()
    structuralPoints: StructuralPoint[];

    @ManyToOne(type => Project, project => project.crossSections)
    project: Project;
}
