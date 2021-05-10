import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated } from "typeorm";
import { Project } from "./Project";

@Entity()
export class StructuralPoint {

    @PrimaryGeneratedColumn()
    id: number;

    @Column()
    name: string;

    @Column()
    @Generated("uuid")
    uuid: string;

    @Column()
    coordinate_x: number;

    @Column()
    coordinate_y: number;

    @Column()
    coordinate_z: number;

    @ManyToOne(type => Project, project => project.structuralPoints)
    project: Project;
}