import { Entity, PrimaryGeneratedColumn, Column, OneToMany } from "typeorm";
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

    @OneToMany(type => Material, material => material.project)
    materials: Material[];

    @OneToMany(type => StructuralPoint, structuralpoint => structuralpoint.project)
    structuralPoints: StructuralPoint[];
}
