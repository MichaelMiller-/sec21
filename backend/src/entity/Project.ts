import { Entity, PrimaryGeneratedColumn, Column, OneToMany } from "typeorm";
import { StructuralPoint } from "./StructuralPoint";

@Entity()
export class Project {

    @PrimaryGeneratedColumn()
    id: number;

    @Column({ nullable: false })
    name: string;

    @Column()
    title: string;

    @OneToMany(type => StructuralPoint, structuralpoint => structuralpoint.project)
    structuralPoints: StructuralPoint[];
}
