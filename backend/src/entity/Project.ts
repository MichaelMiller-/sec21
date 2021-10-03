import {Entity, PrimaryGeneratedColumn, Column, OneToMany, CreateDateColumn, UpdateDateColumn} from "typeorm";
import { CrossSection } from "./CrossSection";
import { CurveMember } from "./CurveMember";
import { LoadCase } from "./LoadCase";
import { Material } from "./Material";
import { StructuralPoint } from "./StructuralPoint";

@Entity()
export class Project {

    @PrimaryGeneratedColumn()
    id: number;

    @Column()
    name: string;

    @Column()
    title: string;

    @CreateDateColumn()
    createdAt: Date;

    @Column()
    modifiedAt: Date; // modified by database trigger

    @OneToMany(type => Material, material => material.project)
    materials: Material[];

    @OneToMany(type => StructuralPoint, structuralpoint => structuralpoint.project)
    structuralPoints: StructuralPoint[];

    @OneToMany(type => CrossSection, crosssection => crosssection.project)
    crossSections: CrossSection[];

    @OneToMany(type => LoadCase, loadcase => loadcase.project)
    loadCases: LoadCase[];

    @OneToMany(type => CurveMember, curvemember => curvemember.project)
    curveMembers: CurveMember[];
}
