import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated } from "typeorm";

@Entity()
export class LoadGroup {

   @PrimaryGeneratedColumn()
   id: number;

   @Column()
   name: string;

   @Column()
   @Generated("uuid")
   uuid: string;

   @Column()
   description: string;

}