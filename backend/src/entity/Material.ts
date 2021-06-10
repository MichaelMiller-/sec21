import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated } from "typeorm";
import { Project } from "./Project";

@Entity()
export class Material {

   @PrimaryGeneratedColumn()
   id: number;

   @Column()
   name: string;

   @Column()
   @Generated("uuid")
   uuid: string;

   @Column()
   E_modulus: number;

   @ManyToOne(type => Project, project => project.materials)
   project: Project;
}