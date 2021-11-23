import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated, ManyToMany } from "typeorm";
import { Project } from "./Project";
import { SurfaceMember } from "./SurfaceMember";

@Entity()
export class StructuralPoint {

    @PrimaryGeneratedColumn()
    id: number;

    @Column()
    name: string;

    @Column()
    @Generated("uuid")
    uuid: string;

    @Column("real")
    coordinate_x: number;

    @Column("real")
    coordinate_y: number;

    @Column("real")
    coordinate_z: number;

    @ManyToOne(type => Project, project => project.structuralPoints)
    project: Project;

    @ManyToMany(type => SurfaceMember, surface => surface.id)
    surfaceMember: SurfaceMember;
}